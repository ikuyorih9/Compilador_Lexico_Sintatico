#ifndef FUNCAO_SAIDA_H
#define FUNCAO_SAIDA_H

#include <stdio.h>

#define NUM_ESTADOS_SAIDA 22

//Tipo de dado que associa um estado, uma função e um valor de retroceder.
typedef struct{
    char estado[4];
    char * (*funcaoSaida)(char*);
    int retrocede;
} Funcao;

int procuraFuncaoSaida(char * estadoFinal, char * token, char ** saida);

//FUNÇÕES DE SAÍDA.
char * funcaoSaida0(char * palavra);
char * funcaoSaida1(char * palavra);
char * funcaoSaida2(char * palavra);
char * funcaoSaida3(char * palavra);
char * funcaoSaida4(char * palavra);
char * funcaoSaida5(char * palavra);
char * funcaoSaida6(char * palavra);
char * funcaoSaida7(char * palavra);
char * funcaoSaida8(char * palavra);
char * funcaoSaida9(char * palavra);
char * funcaoSaida10(char * palavra);
char * funcaoSaida11(char * palavra);
char * funcaoSaida12(char * palavra);
char * funcaoSaida13(char * palavra);
char * funcaoSaida14(char * palavra);
char * funcaoSaida15(char * palavra);
char * funcaoSaida16(char * palavra);
char * funcaoSaida17(char * palavra);
char * funcaoSaida18(char * palavra);
char * funcaoSaida19(char * palavra);

//FUNÇÕES DE ERRO.
char * funcaoErro1(char * palavra);
char * funcaoErro2(char * palavra);
#endif