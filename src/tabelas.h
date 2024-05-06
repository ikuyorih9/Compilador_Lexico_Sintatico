#ifndef TABELAS_H
#define TABELAS_H

#define MAX_PALAVRA 1024
#define MAX_TABELA 124
#define NUM_PALAVRAS_RESERVADAS 11
#define ESTADO_INICIAL "Q0"

#define TRANSICAO_PATH "transicoes.txt"
#define PALAVRAS_RESERVADAS_PATH "../palavras_reservadas.txt"

typedef struct{
    char estadoCorrente[4];
    char entrada;
    char estadoProx[4];
} Transicao;

// const Transicao tabelaTransicao[] = {
//     {{'Q','0'}, '_', {'Q','1'}},
//     {{'Q','0'}, 'a', {'Q','1'}},
//     {{'Q','0'}, 'b', {'Q','1'}},
//     {{'Q','0'}, 'c', {'Q','1'}},
//     {{'Q','0'}, 'd', {'Q','1'}},
//     {{'Q','0'}, 'e', {'Q','1'}},
//     {{'Q','0'}, 'f', {'Q','1'}},
//     {{'Q','0'}, 'g', {'Q','1'}},
//     {{'Q','0'}, 'h', {'Q','1'}},
//     {{'Q','0'}, 'i', {'Q','1'}},
//     {{'Q','0'}, 'j', {'Q','1'}},
//     {{'Q','0'}, 'k', {'Q','1'}},
//     {{'Q','0'}, 'l', {'Q','1'}},
//     {{'Q','0'}, 'm', {'Q','1'}},
//     {{'Q','0'}, 'n', {'Q','1'}},
//     {{'Q','0'}, 'o', {'Q','1'}},
//     {{'Q','0'}, 'p', {'Q','1'}},
//     {{'Q','0'}, 'q', {'Q','1'}},
//     {{'Q','0'}, 'r', {'Q','1'}},
//     {{'Q','0'}, 's', {'Q','1'}},
//     {{'Q','0'}, 't', {'Q','1'}},
//     {{'Q','0'}, 'u', {'Q','1'}},
//     {{'Q','0'}, 'v', {'Q','1'}},
//     {{'Q','0'}, 'w', {'Q','1'}},
//     {{'Q','0'}, 'x', {'Q','1'}},
//     {{'Q','0'}, 'y', {'Q','1'}},
//     {{'Q','0'}, 'z', {'Q','1'}},
//     {{'Q','0'}, 'A', {'Q','1'}},
//     {{'Q','0'}, 'B', {'Q','1'}},
//     {{'Q','0'}, 'C', {'Q','1'}},
//     {{'Q','0'}, 'D', {'Q','1'}},
//     {{'Q','0'}, 'E', {'Q','1'}},
//     {{'Q','0'}, 'F', {'Q','1'}},
//     {{'Q','0'}, 'G', {'Q','1'}},
//     {{'Q','0'}, 'H', {'Q','1'}},
//     {{'Q','0'}, 'I', {'Q','1'}},
//     {{'Q','0'}, 'J', {'Q','1'}},
//     {{'Q','0'}, 'K', {'Q','1'}},
//     {{'Q','0'}, 'L', {'Q','1'}},
//     {{'Q','0'}, 'M', {'Q','1'}},
//     {{'Q','0'}, 'N', {'Q','1'}},
//     {{'Q','0'}, 'O', {'Q','1'}},
//     {{'Q','0'}, 'P', {'Q','1'}},
//     {{'Q','0'}, 'Q', {'Q','1'}},
//     {{'Q','0'}, 'R', {'Q','1'}},
//     {{'Q','0'}, 'S', {'Q','1'}},
//     {{'Q','0'}, 'T', {'Q','1'}},
//     {{'Q','0'}, 'U', {'Q','1'}},
//     {{'Q','0'}, 'V', {'Q','1'}},
//     {{'Q','0'}, 'W', {'Q','1'}},
//     {{'Q','0'}, 'X', {'Q','1'}},
//     {{'Q','0'}, 'Y', {'Q','1'}},
//     {{'Q','0'}, 'Z', {'Q','1'}},
//     {{'Q','0'}, 'Z', {'Q','1'}},
//     {{'Q','1'}, '0', {'Q','1'}},
//     {{'Q','1'}, '1', {'Q','1'}},
//     {{'Q','1'}, '2', {'Q','1'}},
//     {{'Q','1'}, '3', {'Q','1'}},
//     {{'Q','1'}, '4', {'Q','1'}},
//     {{'Q','1'}, '5', {'Q','1'}},
//     {{'Q','1'}, '6', {'Q','1'}},
//     {{'Q','1'}, '7', {'Q','1'}},
//     {{'Q','1'}, '8', {'Q','1'}},
//     {{'Q','1'}, '9', {'Q','1'}},
//     {{'Q','1'}, '_', {'Q','1'}},
//     {{'Q','1'}, 'a', {'Q','1'}},
//     {{'Q','1'}, 'b', {'Q','1'}},
//     {{'Q','1'}, 'c', {'Q','1'}},
//     {{'Q','1'}, 'd', {'Q','1'}},
//     {{'Q','1'}, 'e', {'Q','1'}},
//     {{'Q','1'}, 'f', {'Q','1'}},
//     {{'Q','1'}, 'g', {'Q','1'}},
//     {{'Q','1'}, 'h', {'Q','1'}},
//     {{'Q','1'}, 'i', {'Q','1'}},
//     {{'Q','1'}, 'j', {'Q','1'}},
//     {{'Q','1'}, 'k', {'Q','1'}},
//     {{'Q','1'}, 'l', {'Q','1'}},
//     {{'Q','1'}, 'm', {'Q','1'}},
//     {{'Q','1'}, 'n', {'Q','1'}},
//     {{'Q','1'}, 'o', {'Q','1'}},
//     {{'Q','1'}, 'p', {'Q','1'}},
//     {{'Q','1'}, 'q', {'Q','1'}},
//     {{'Q','1'}, 'r', {'Q','1'}},
//     {{'Q','1'}, 's', {'Q','1'}},
//     {{'Q','1'}, 't', {'Q','1'}},
//     {{'Q','1'}, 'u', {'Q','1'}},
//     {{'Q','1'}, 'v', {'Q','1'}},
//     {{'Q','1'}, 'w', {'Q','1'}},
//     {{'Q','1'}, 'x', {'Q','1'}},
//     {{'Q','1'}, 'y', {'Q','1'}},
//     {{'Q','1'}, 'z', {'Q','1'}},
//     {{'Q','1'}, 'A', {'Q','1'}},
//     {{'Q','1'}, 'B', {'Q','1'}},
//     {{'Q','1'}, 'C', {'Q','1'}},
//     {{'Q','1'}, 'D', {'Q','1'}},
//     {{'Q','1'}, 'E', {'Q','1'}},
//     {{'Q','1'}, 'F', {'Q','1'}},
//     {{'Q','1'}, 'G', {'Q','1'}},
//     {{'Q','1'}, 'H', {'Q','1'}},
//     {{'Q','1'}, 'I', {'Q','1'}},
//     {{'Q','1'}, 'J', {'Q','1'}},
//     {{'Q','1'}, 'K', {'Q','1'}},
//     {{'Q','1'}, 'L', {'Q','1'}},
//     {{'Q','1'}, 'M', {'Q','1'}},
//     {{'Q','1'}, 'N', {'Q','1'}},
//     {{'Q','1'}, 'O', {'Q','1'}},
//     {{'Q','1'}, 'P', {'Q','1'}},
//     {{'Q','1'}, 'Q', {'Q','1'}},
//     {{'Q','1'}, 'R', {'Q','1'}},
//     {{'Q','1'}, 'S', {'Q','1'}},
//     {{'Q','1'}, 'T', {'Q','1'}},
//     {{'Q','1'}, 'U', {'Q','1'}},
//     {{'Q','1'}, 'V', {'Q','1'}},
//     {{'Q','1'}, 'W', {'Q','1'}},
//     {{'Q','1'}, 'X', {'Q','1'}},
//     {{'Q','1'}, 'Y', {'Q','1'}},
//     {{'Q','1'}, 'Z', {'Q','1'}},
//     {{'Q','1'}, 'Z', {'Q','1'}},

//     {{'Q','1'}, '@', {'Q','2'}},
//     {{'Q','1'}, '?', {'Q','2'}},
//     {{'Q','1'}, '#', {'Q','2'}},
//     {{'Q','1'}, ' ', {'Q','2'}},
//     {{'Q','1'}, ',', {'Q','2'}},
//     {{'Q','1'}, ';', {'Q','2'}},
//     {{'Q','1'}, '.', {'Q','2'}}
// };

// const char tabelaPalavrasReservadas[NUM_PALAVRAS_RESERVADAS][10] = {
//     {'P','R','O','C','E','D','U','R','E'},
//     {'B','E','G','I','N'},
//     {'W','H','I','L','E'},
//     {'C','O','N','S','T'},
//     {'C','A','L','L'},
//     {'T','H','E','N'},
//     {'V','A', 'R'},
//     {'O','D', 'D'},
//     {'E','N','D'},
//     {'D','O'},
//     {'I','F'}
// };

Transicao buscaTabelaTransicoes(char * estadoAtual, char entrada);
int estadoFinal(char * estado);

#endif