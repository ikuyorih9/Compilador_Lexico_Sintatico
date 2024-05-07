#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "analisador_lexico.h"

//Realiza a análise léxica do programa de entrada.
/*
Realiza a análise léxica do programa de entrada.
@param entrada arquivo de entrada com o programa PL/0.
@return 
*/
int analisadorLexico(FILE * entrada, FILE * saida){
    char estadoAtual[3] = {'Q','0'};
    int posPalavra = 0;

    char * linha = (char*) malloc(MAX_LINHA);
    char * palavra = (char*) malloc(MAX_PALAVRA);
    while(!feof(entrada)){
        fgets(linha, MAX_LINHA, entrada);
        dprint("\nLINHA: %s", linha);
        for(int i = 0; linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'; i++){
            if(linha[i] == '\n' || linha[i] == '\t')
                continue;

            palavra[posPalavra] = linha[i];
            palavra[posPalavra+1] = '\0';
            posPalavra++;
            
            dprint("\tTabela de transicao:  (%s, %c) = ", estadoAtual, linha[i]);

            Transicao t = buscaTabelaTransicoes(estadoAtual, linha[i]);
            if(t.entrada == -1){
                dprint("Transição não encontrada.\n");
                exit(0);
            }
            strcpy(estadoAtual, t.estadoProx);
            dprint("%s.\n", estadoAtual);
            if(estadoFinal(estadoAtual)){
                dprint("\t\tChamando funcao de saida para %s...\n", estadoAtual);

                int retrocede = procuraFuncaoSaida(estadoAtual, palavra, saida);
                if(retrocede){
                    dprint("\t\tRetrocedendo \'%c\'...\n", linha[i]);
                    i--;
                }
                posPalavra = 0;
                strcpy(estadoAtual, ESTADO_INICIAL);
                continue;
            }
        } 
    }
    free(palavra);
    free(linha);
}