#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"

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

    analisadorLexico(entrada, saida);

    fclose(entrada);
    fclose(saida);
    return 0;
}
