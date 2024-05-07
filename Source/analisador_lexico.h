#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO

#include <stdio.h>

//Imprime a mensagems somente se DEBUG_PRINT = 1.
#define dprint if(DEBUG_PRINT) printf
//Controle das mensagens de debug.
#define DEBUG_PRINT 1

int analisadorLexico(FILE * entrada, FILE * saida);

#endif