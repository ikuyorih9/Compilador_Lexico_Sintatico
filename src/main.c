#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"

#define ENTRADA_PATH "in.txt"
#define SAIDA_PATH "out.txt"

#define DEBUG_PRINT 1
#define dprint if(DEBUG_PRINT) printf

int main(){
    char estadoAtual[3] = {'Q','0'};
    int posPalavra = 0;

    FILE * entrada = fopen(ENTRADA_PATH, "r");
    if(!entrada){
        printf("O arquivo %s nao existe. Saindo...\n", ENTRADA_PATH);
        exit(0);
    }

    FILE * saida = fopen(SAIDA_PATH, "w");
    if(!saida){
        printf("O arquivo %s nao foi criado. Saindo...\n", SAIDA_PATH);
        exit(0);
    }

    char linha[1024];
    char * palavra = (char*) malloc(MAX_PALAVRA);
    while(!feof(entrada)){
        fgets(linha, 1024, entrada);
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
                printf("Transição não encontrada.\n");
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
    fclose(entrada);
    fclose(saida);
    return 0;
}
