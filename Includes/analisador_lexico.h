#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO

#include <stdio.h>
#include "token.h"

Token * analisadorLexico(char * linha, int * i, FILE * entrada);

void obterSimbolo(FILE * entrada, char * linha, int *i, Token ** token);

#endif