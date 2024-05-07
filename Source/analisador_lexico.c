#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "analisador_lexico.h"

//Realiza a análise léxica do programa de entrada.
/*
Realiza a análise léxica do programa de entrada.
@param posicao
@return 
*/

char * analisadorLexico(int * pos, char * linha){
    int i = *pos;
    int posPalavra = 0;
    char estadoAtual[3] = {'Q','0'};
    char * palavra = (char*) malloc(MAX_PALAVRA);

    while(linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'){
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
            dprint("\t\tEnviando palavra %s...\n", palavra);
            char * token;
            int retrocede = procuraFuncaoSaida(estadoAtual, palavra, &token);
            if(retrocede){
                dprint("\t\tRetrocedendo \'%c\'...\n", linha[i]);
                i--;
            }
            posPalavra = 0;
            strcpy(estadoAtual, ESTADO_INICIAL);
            *pos = i;
            return token;
        }

        i++;
    }
    return NULL;
}