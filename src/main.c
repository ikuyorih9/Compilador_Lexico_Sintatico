#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"
#include "tabelas.h"

#define CODIGO_PATH "in.txt"

int main(){
    char estadoAtual[3] = {'Q','0','\0'};
    char palavra[MAX_PALAVRA];
    int posPalavra = 0;

    FILE * codigo = fopen(CODIGO_PATH, "r");
    if(!codigo){
        printf("O arquivo %s nao existe. Saindo...\n", CODIGO_PATH);
        exit(0);
    }

    char linha[1024];
    while(!feof(codigo)){
        fgets(linha, 1024, codigo);
        for(int i = 0; linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'; i++){
            if(linha[i] == '\n')
                continue;

            Transicao t = buscaTabelaTransicoes(estadoAtual, linha[i]);
            if(t.entrada == -1){
                printf("Transição não encontrada.\n");
                exit(0);
            }

            strcpy(estadoAtual, t.estadoProx);
            if(estadoFinal(estadoAtual)){
                i--; //Retroceder.
                printf("%s, %s\n", palavra, palavra);
                posPalavra = 0;
                strcpy(estadoAtual, ESTADO_INICIAL);
                continue;
            }

            palavra[posPalavra] = linha[i];
            palavra[posPalavra+1] = '\0';
            posPalavra++;

        }
        
    }

    


    // for(int posEntrada = 0; entrada[posEntrada] != '\0'; posEntrada++){
    //     palavra[posPalavra] = entrada[posEntrada]; //Atualiza a palavra lida.
    //     printf("%c\n", palavra[posPalavra]);

        // strcpy(estadoAtual, tabelaTransicao[indiceProxEstado].estadoProx); //Atualiza o estado atual para o próximo estado.
        // int fim = !strcmp(estadoAtual, "Q2"); //Verifica se o estado atual é o fim.

        // if(fim){
        //     posEntrada--;
        //     palavra[posPalavra] = '\0';
        //     if(verificaPalavraReservada(palavra)){
        //         printf("%s, %s", palavra, palavra);
        //     }
        //     strcpy(estadoAtual, ESTADO_INICIAL); //Volta ao estado inicial.
        //     posPalavra = 0;
        // }
        // else
        //     posPalavra++;
    //}
    return 0;
}