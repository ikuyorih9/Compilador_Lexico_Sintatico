#ifndef FUNCAO_SAIDA_H
#define FUNCAO_SAIDA_H

#include <stdio.h>
#include "token.h"

#define NUM_ESTADOS_SAIDA 22

//Tipo de dado que associa um estado, uma função e um valor de retroceder.
typedef struct{
    char estado[4];
    Token * (*funcaoSaida)(char*);
    int retrocede;
} Funcao;

int procuraFuncaoSaida(char * estadoFinal, char * token, Token ** saida);

//FUNÇÕES DE SAÍDA.
Token * funcaoSaida0(char * palavra);
Token * funcaoSaida1(char * palavra);
Token * funcaoSaida2(char * palavra);
Token * funcaoSaida3(char * palavra);
Token * funcaoSaida4(char * palavra);
Token * funcaoSaida5(char * palavra);
Token * funcaoSaida6(char * palavra);
Token * funcaoSaida7(char * palavra);
Token * funcaoSaida8(char * palavra);
Token * funcaoSaida9(char * palavra);
Token * funcaoSaida10(char * palavra);
Token * funcaoSaida11(char * palavra);
Token * funcaoSaida12(char * palavra);
Token * funcaoSaida13(char * palavra);
Token * funcaoSaida14(char * palavra);
Token * funcaoSaida15(char * palavra);
Token * funcaoSaida16(char * palavra);
Token * funcaoSaida17(char * palavra);
Token * funcaoSaida18(char * palavra);
Token * funcaoSaida19(char * palavra);

//FUNÇÕES DE ERRO.
Token * funcaoErro1(char * palavra);
Token * funcaoErro2(char * palavra);
#endif