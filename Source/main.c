#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"
#include "tabelas.h"
#include "simbolos.h"
#include "proc_sintaticos.h"
#include "configs.h"

//Arquivo de entrada PL/0 padrão.
#define ENTRADA_PADRAO_PATH "Entradas/in.txt"
//Arquivo de saída padrão.
#define SAIDA_PATH "Logs/out.txt"

int main(int argc, char * argv[]){
    //Abertura do arquivo de entrada para leitura.
    FILE * entrada;

    //Se o número de argumentos de entrada inclui o arquivo de entrada;
    if(argc == 2){
        entrada = fopen(argv[1], "r");
        dprint("Lendo arquivo de entrada '%s'...\n", argv[1]);   
        if(!entrada){
            printf("O arquivo %s nao existe. Saindo...\n", argv[1]);
            exit(-1);
        }
    }
    //Se o número de argumentos de entrada NÃO inclui o arquivo de entrada;
    else if(argc == 1){
        entrada = fopen(ENTRADA_PADRAO_PATH, "r");
        dprint("Lendo arquivo de entrada '%s'...\n", ENTRADA_PADRAO_PATH);
        if(!entrada){
            printf("O arquivo %s nao existe. Saindo...\n", ENTRADA_PADRAO_PATH);
            exit(-1);
        }
    }
    //Se o número de argumentos é diferente de qualquer outro.
    else{
        printf("%d\n", argc);
        printf("Número de parâmetros errado.");
        exit(-1);
    }

    //Abertura do arquivo de saída para escrita.
    FILE * saida = fopen(SAIDA_PATH, "w");
    if(!saida){
        printf("O arquivo %s nao foi criado. Saindo...\n", SAIDA_PATH);
        exit(0);
    }

    //listaTokens(entrada, saida);

    //Aloca dinamicamente uma linha para ler da entrada.
    char * linha = (char*) malloc(MAX_LINHA);
    int i = 0;
    linha[0] = '\0';
    


    int num_erros = 0;
    Token * token = NULL;
    obterSimbolo(entrada, linha, &i, &token);
    //p_programa(entrada, linha, &i, &token, &num_erros);
    //printf("fim com token: (%s,%s)\n", (token)->valor, (token)->tipo);

    while(!feof(entrada) && token != NULL){
        p_programa(entrada, linha, &i, &token, &num_erros);
        //printf("fim 2 com token: (%s,%s)\n", (token)->valor, (token)->tipo);
        obterSimbolo(entrada, linha, &i, &token);
    }
    destroiToken(token);
    free(linha); //Libera a memória alocada para a linha.

    // if(num_erros == 0){
    //     printf("Não foram encontrados erros. Compilação terminou com sucesso.\n");
    // }else{
    //     printf("Foram encontrados %d erros\n", num_erros);
    // }

    //Libera os arquivos de entrada e saída.
    fclose(entrada);
    fclose(saida);
    return 0;
}
