#include <stdio.h>
#include "tabela.h"
#include <string.h>

int buscaTabela(char * estadoCorrente, char entrada){
    for(int i = 0; i < 124; i++){
        char * estado = tabela[i].estadoCorrente;
        char myentrada = tabela[i].entrada;
        if(estado[0] == estadoCorrente[0] && estado[1] == estadoCorrente[1] && myentrada == entrada)
            return i;
    }
    return -1;
}

int main(){
    char entrada[] = {'V','A','R',' ','a',',','b',',','c',';'};
    char estadoInicial[3] = {'Q','0'};
    int fim = 0;
    int pos = 0;

    for(int pos = 0; entrada[pos] != '\0'; pos++){
        int i = buscaTabela(estadoInicial,entrada[pos]);
        printf("Estou: %s, li %c, vou %s (%d)\n",tabela[i].estadoCorrente, tabela[i].entrada, tabela[i].estadoProx , i);

        strcpy(estadoInicial, tabela[i].estadoProx);

        if(estadoInicial[1] == '2'){
            printf("Voltando...\n");
            estadoInicial[1] = '0';
        }
    }
    return 0;
}