#ifndef TABELAS_H
#define TABELAS_H

#define MAX_PALAVRA 256
#define MAX_LINHA 1024
#define NUM_PALAVRAS_RESERVADAS 11
#define ESTADO_INICIAL "Q0"

#define TRANSICAO_PATH "Tabelas/transicoes.txt"
#define PALAVRAS_RESERVADAS_PATH "Tabelas/palavras_reservadas.txt"

//Imprime a mensagems somente se DEBUG_PRINT = 1.
#define dprint if(DEBUG_PRINT) printf
//Controle das mensagens de debug.
#define DEBUG_PRINT 0

typedef struct{
    char estadoCorrente[4];
    char entrada;
    char estadoProx[4];
} Transicao;

Transicao buscaTabelaTransicoes(char * estadoAtual, char entrada);
int verificaSePalavraReservada(char * palavra);
int estadoFinal(char * estado);


#endif