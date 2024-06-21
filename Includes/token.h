#ifndef TOKEN_H
#define TOKEN_H

#include "tabelas.h"

/*
Tipo de dado que representa um token
@param valor (char *) representa o valor do token. 
@param tipo (char *) representa o tipo do token. 
*/
typedef struct{
    char * valor;
    char * tipo;
} Token;

/*
Aloca memória para um tipo Token*.
@return (Token*) token inicializado.
*/
Token * inicializaToken();

/*
Desaloca memória para um token.
@param token (Token*) a ser desalocado.
*/
void destroiToken(Token * token);

/*
Compara token recebido com um símbolo.
@param token Token a comparar.
@param simbolo Símbolo a comparar.
*/
int cmpToken(Token * token, char * simbolo);


#endif