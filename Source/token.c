#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

/*
Aloca memória para um tipo Token*.
@return (Token*) token inicializado.
*/
Token * inicializaToken(){
    Token * token = (Token *) malloc(sizeof(Token));
    if(token == NULL){
        printf("Erro na alocação do token.\n");
        return NULL;
    }

    token->valor = (char*) malloc(MAX_LINHA);
    token->tipo = (char*) malloc(MAX_LINHA);
    return token;
}

/*
Desaloca memória para um token.
@param token (Token*) a ser desalocado.
*/
void destroiToken(Token * token){
    if(token != NULL){
        if(token->tipo != NULL)
            free(token->tipo);
        if(token->valor != NULL)
            free(token->valor);
        free(token);
    }
    else{
        dprint("Token ja esta desalocado.\n");
    }
    token = NULL;
}

/*
Compara token recebido com um símbolo.
@param token Token a comparar.
@param simbolo Símbolo a comparar.
*/
int cmpToken(Token * token, char * simbolo){
    return !strcmp(token->tipo, simbolo);
}