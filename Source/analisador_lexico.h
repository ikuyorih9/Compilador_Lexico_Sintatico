#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO

#include <stdio.h>

char * analisadorLexico(char * linha, int * i);

char * obterSimbolo(FILE * entrada, char * linha, int *i);

#endif