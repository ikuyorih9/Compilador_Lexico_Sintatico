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
    printf("Procurando por %s com entrada \'%c\'\n", estadoAtual, entrada);
    FILE * tabelaTransicoes = fopen(TRANSICAO_PATH, "r");
    if(!tabelaTransicoes){
        printf("O arquivo %s nao existe. Saindo...\n", TRANSICAO_PATH);
        exit(0);
    }

    char palavra[MAX_PALAVRA];
    Transicao transicaoSaida = {{'\0'}, -1, {'\0'}};
    while(!feof(tabelaTransicoes)){
        Transicao t;
        int i = 0;
        int espacos = 0;

        char str[MAX_PALAVRA];
        fgets(str,MAX_PALAVRA,tabelaTransicoes);
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
    fclose(tabelaTransicoes);
    return transicaoSaida;
}

int estadoFinal(char * estado){
    if(estado[0] == 'S')
        return 1;
    return 0;
}


