#include <stdio.h>
#include <stdlib.h>
#include "token.h"

/*
Aloca memória para um tipo Token*.
@return (Token*) token inicializado.
*/
Token * inicializaToken(){
    Token * token = (Token *) malloc(sizeof(Token));
    token->valor = (char*) malloc(MAX_LINHA);
    token->tipo = (char*) malloc(MAX_LINHA);
    return token;
}

/*
Desaloca memória para um token.
@param token (Token*) a ser desalocado.
*/
void destroiToken(Token * token){
    free(token->tipo);
    free(token->valor);
    free(token);
}