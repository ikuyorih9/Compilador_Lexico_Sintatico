#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"
#include "tabelas.h"

#define ENTRADA_PATH "Logs/in.txt"
#define SAIDA_PATH "Logs/out.txt"

int main(){
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

    char * linha = (char*) malloc(MAX_LINHA);
    while(!feof(entrada)){
        fgets(linha, MAX_LINHA, entrada);
        dprint("LINHA: %s\n", linha);
        int tamLinha = strlen(linha);
        for(int i = 0; linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'; i++){
            dprint("\tComecei na posição linha[%d].\n", i);
            char * token = pseudoLexico(&i, linha);
            dprint("\tVoltei na posição linha[%d].\n", i);
            if(token!=NULL){
                fprintf(saida,"%s\n", token);
                printf("TOKEN: %s\n",token);
                free(token);
            }
        }
    }

    fclose(entrada);
    fclose(saida);
    return 0;
}
