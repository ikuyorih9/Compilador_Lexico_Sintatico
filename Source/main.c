#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"
#include "tabelas.h"

#define ENTRADA_PADRAO_PATH "Entradas/in.txt"
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

    //Aloca dinamicamente uma linha para ler da entrada.
    char * linha = (char*) malloc(MAX_LINHA);

    //Enquanto o arquivo de entrada não estiver em EOF:
    while(!feof(entrada)){
        //Lê uma linha da entrada;
        fgets(linha, MAX_LINHA, entrada);
        dprint("\nLINHA: %s\n", linha);

        //Percorre toda a linha com o iterador 'i', evitando caracteres inválidos da linha.
        int i = 0;
        while(linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'){
            dprint("Comecei na posição linha[%d].\n", i);
            
            //Chama o analisador léxico para receber um TOKEN da linha e atualiza o iterador 'i' para a posição onde ele parou.
            char * token = analisadorLexico(linha, &i);
            dprint("Voltei na posição linha[%d].\n", i);

            //Se o valor do token for válido, imprime o token no arquivo e no terminal.
            if(token!=NULL){
                fprintf(saida,"%s\n", token);
                printf("\nTOKEN: %s\n\n",token);
                free(token);
            }

            //Avança na linha.
            i++;
        }
    }

    free(linha); //Libera a memória alocada para a linha.

    //Libera os arquivos de entrada e saída.
    fclose(entrada);
    fclose(saida);
    return 0;
}
