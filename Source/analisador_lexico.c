#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "analisador_lexico.h"

/*
Realiza a análise léxica do primeiro token encontrado numa linha de entrada.
@param linha string da linha lida
@param pos posição da linha lida;
@return retorna o par (token, identificação). Se NULL, foi encontrado um espaço vazio.
*/
Token * analisadorLexico(char * linha, int * pos, FILE * entrada){
    int i = *pos;
    int posPalavra = 0;
    char estadoAtual[3] = {'Q','0'};
    char * palavra = (char*) malloc(MAX_PALAVRA);

    int fimDeLinha = 0;

    //Enquanto a linha não encontrar um caractere inválido da linha.
    // while(linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r' && linha[i] != '\t'){
    //Percorre a linha.
    while(!fimDeLinha){
        //Salva o caractere na 'palavra'.
        palavra[posPalavra] = linha[i];
        palavra[posPalavra+1] = '\0';
        posPalavra++;

        dprint("\tTabela de transicao:  (%s, %c) = ", estadoAtual, linha[i]);

        //Busca a transição do caractere 'linha[i]', estando no estado 'estadoAtual'. Também verifica se a transição existe.
        Transicao t = buscaTabelaTransicoes(estadoAtual, linha[i]);
        if(t.entrada == -1){
            dprint("Transição não encontrada.\n");
            exit(0);
        }

        //Atualiza o estado atual, avançando no autômato.
        strcpy(estadoAtual, t.estadoProx);
        dprint("%s.\n", estadoAtual);

        //Se avançou para um estado final.
        if(estadoFinal(estadoAtual)){
            dprint("\t\tChamando funcao de saida para %s...\n", estadoAtual);
            dprint("\t\tEnviando palavra %s...\n", palavra);

            //Procura a função de saída para o estado final, salvando em 'token' a resposta do analisador léxico e retornando se deve retroceder.
            Token * token;
            int retrocede = procuraFuncaoSaida(estadoAtual, palavra, &token);

            if(token != NULL)
                dprint("\t\tToken encontrado: (%s, %s).\n", token->valor, token->tipo);

            //Se tiver que retroceder, o iterador volta uma posição, para que o último caractere seja lido de novo.
            if(retrocede){
                dprint("\t\tRetrocedendo \'%c\'...\n", linha[i]);
                i--;
                if(feof(entrada))
                    fseek(entrada, -1, SEEK_CUR);
            }

            //Atualiza o iterador para a posição atual, que avançou na linha.
            *pos = i;

            //Retorna o token lido.
            return token;
        }
        i++;
    }
    return NULL;
}

void obterSimbolo(FILE * entrada, char * linha, int *i, Token ** token){
    destroiToken(*token);
    //Enquanto o arquivo de entrada não estiver em EOF:
    while(!feof(entrada)){
        //Lê uma linha da entrada;
        if(strlen(linha) == 0){
            fgets(linha, MAX_LINHA, entrada);
            dprint("\nLINHA: %s\n", linha);
        }

        //Percorre toda a linha com o iterador 'i', evitando caracteres inválidos da linha.
        while(linha[*i] != '\n' && linha[*i] != '\0' && linha[*i] != '\r'){
            dprint("Comecei na posição linha[%d].\n", *i);
            
            //Chama o analisador léxico para retornar um TOKEN da linha e atualiza o iterador 'i' para a posição onde ele parou.
            *token = analisadorLexico(linha, i, entrada);
            if(*token == NULL){
                dprint("Caractere ignorado...\n");
            }
            //dprint("\t\tToken encontrado: (%s, %s).\n", (*token)->valor, (*token)->tipo);
            dprint("Voltei na posição linha[%d].\n", *i);

            //Avança na linha.
            (*i)++;

            if(*token != NULL){
                return;
            }
            
            
        }

        (*i) = 0;
        linha[*i] = '\0';
    }    
}