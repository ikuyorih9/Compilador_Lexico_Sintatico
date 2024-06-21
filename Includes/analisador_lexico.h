#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO

#include <stdio.h>
#include "funcoes_saida.h"

Token * analisadorLexico(char * linha, int * i);

Token * obterSimbolo(FILE * entrada, char * linha, int *i);

#endif