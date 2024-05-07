#ifndef FUNCAO_SAIDA_H
#define FUNCAO_SAIDA_H

#include <stdio.h>

#define NUM_ESTADOS_SAIDA 22

//Tipo de dado que associa um estado, uma função e um valor de retroceder.
typedef struct{
    char estado[4];
    void (*funcaoSaida)(char*,FILE*);
    int retrocede;
} Funcao;

int procuraFuncaoSaida(char * estadoFinal, char * palavra, FILE * saida);

//FUNÇÕES DE SAÍDA.
void funcaoSaida0(char * palavra, FILE * saida);
void funcaoSaida1(char * palavra, FILE * saida);
void funcaoSaida2(char * palavra, FILE * saida);
void funcaoSaida3(char * palavra, FILE * saida);
void funcaoSaida4(char * palavra, FILE * saida);
void funcaoSaida5(char * palavra, FILE * saida);
void funcaoSaida6(char * palavra, FILE * saida);
void funcaoSaida7(char * palavra, FILE * saida);
void funcaoSaida8(char * palavra, FILE * saida);
void funcaoSaida9(char * palavra, FILE * saida);
void funcaoSaida10(char * palavra, FILE * saida);
void funcaoSaida11(char * palavra, FILE * saida);
void funcaoSaida12(char * palavra, FILE * saida);
void funcaoSaida13(char * palavra, FILE * saida);
void funcaoSaida14(char * palavra, FILE * saida);
void funcaoSaida15(char * palavra, FILE * saida);
void funcaoSaida16(char * palavra, FILE * saida);
void funcaoSaida17(char * palavra, FILE * saida);
void funcaoSaida18(char * palavra, FILE * saida);
void funcaoSaida19(char * palavra, FILE * saida);

//FUNÇÕES DE ERRO.
void funcaoErro1(char * palavra, FILE * saida);
void funcaoErro2(char * palavra, FILE * saida);
#endif