#include "tabelas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Busca o índice do próximo estado na tabela de transição. Ele abre o arquivo no caminho TRANSICAO_PATH.
@param estadoAtual estado atual do autômato.
@param leitura caracter lido pelo autômato.
@return índice do próximo estado. Se não encontrado na tabela, retorna -1.
*/
Transicao buscaTabelaTransicoes(char * estadoAtual, char entrada){
    FILE * tabelaTransicoes = fopen(TRANSICAO_PATH, "r");
    if(!tabelaTransicoes){
        printf("O arquivo %s nao existe. Saindo...\n", TRANSICAO_PATH);
        exit(0);
    }
    
    Transicao transicaoSaida = {{'\0'}, -1, {'\0'}};
    char * str = (char*) malloc(MAX_PALAVRA);
    while(!feof(tabelaTransicoes)){
        Transicao t;
        int i = 0;
        int espacos = 0;

        fgets(str, MAX_PALAVRA, tabelaTransicoes);

        //Ignora os comentáriso da tabela de transição.
        if(str[0] == '/' && str[1] == '/')
            continue;
            
        //Lê uma linha do arquivo da tabela de transição.
        for(int pos = 0; str[pos] != '\0' && str[pos] != '\n' && str[pos] != '\r'; pos++){
            char c = str[pos];
            if(c == ' '){
                espacos++;
                i = 0;
                continue;
            }

            if(espacos == 0){
                t.estadoCorrente[i] = c;
                t.estadoCorrente[i+1] = '\0';
            }
            else if(espacos == 1){
                if(str[pos] == '\\' && str[pos+1] == 's'){
                    t.entrada = ' ';
                    pos++;
                }
                else if(str[pos] == '\\' && str[pos+1] == 'n'){
                    t.entrada = '\n';
                    pos++;
                }
                else if(str[pos] == '\\' && str[pos+1] == 't'){
                    t.entrada = '\t';
                    pos++;
                }
                else if(str[pos] == '\\' && str[pos+1] == 'r'){
                    t.entrada = '\r';
                    pos++;
                }
                else if(str[pos] == '\\' && str[pos+1] == '0'){
                    t.entrada = '\0';
                    pos++;
                }
                else{
                    t.entrada = c;
                }
            }
            else if(espacos == 2){
                t.estadoProx[i] = c;
                t.estadoProx[i+1] = '\0';
            }  
            i++;
        }
        int mesmoEstadoAtual = strcmp(t.estadoCorrente, estadoAtual) == 0;
        int mesmaEntrada = (entrada == t.entrada);
        //printf("%s, %c, %s\n", t.estadoCorrente, t.entrada, t.estadoProx);
        if(mesmoEstadoAtual && mesmaEntrada){
            transicaoSaida = t;
        }
    }
    free(str);
    fclose(tabelaTransicoes);
    return transicaoSaida;
}

/*
Verifica se a palavra é reservada.
@param palavra palavra lida pelo autômato.
@return (0) se não é uma palavra reservada; (1) se é uma palavra reservada.
@note A macro PALAVRAS_RESERVADAS_PATH identifica o caminho do arquivo de palavras reservadas.
*/
int verificaSePalavraReservada(char * palavra){
    //Abre o arquivo de palavras reservadas e verifica se existe.
    FILE * arquivoPalavrasReservadas = fopen(PALAVRAS_RESERVADAS_PATH, "r");
    if(!arquivoPalavrasReservadas){
        printf("O arquivo %s nao existe. Saindo...\n", PALAVRAS_RESERVADAS_PATH);
        exit(0);
    }

    char palavraReservada[MAX_PALAVRA];

    //Percorre todo o arquivo até atingir EOF.
    while(!feof(arquivoPalavrasReservadas)){
        //Lê uma linha do arquivo de palavras reservadas.
        fgets(palavraReservada, MAX_PALAVRA, arquivoPalavrasReservadas);

        //Percorre toda a linha e retira '\n' e '\r'.
        for(int i = 0; i < strlen(palavraReservada); i++){
            if(palavraReservada[i] == '\n' || palavraReservada[i] == '\r'){
                palavraReservada[i] = '\0';
                break;
            }
        }

        //Verifica se a palavra é reservada.
        int encontrouPalavraReservada = !strcmp(palavra, palavraReservada);
        if(encontrouPalavraReservada)
            return 1;
    }
    return 0;
}

/*
Verificar se o estado é final, analisando se é um estado 'S' ou 'E'.
@param estado valor do estado.
@return (0) se não é estado final; (1) se é estado final.
*/
int estadoFinal(char * estado){
    if(estado[0] == 'S' || estado[0] == 'E')
        return 1;
    return 0;
}



