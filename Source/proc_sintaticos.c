#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_sintaticos.h"
#include "analisador_lexico.h"
#include "simbolos.h"

int p_programa(FILE * entrada, char * linha, int *i, char * token, char ** s){
    // O menor programa tem so um ponto final
    if(token != NULL && strcmp(token, SIMB_PONTO) == 0){
        return 0;
    }

    //Chama procedimento do Bloco
    p_bloco(entrada, linha, i, token, s);

    //Depois de um bloco deve vir um ponto final
    if(token != NULL && strcmp(token, SIMB_PONTO) == 0){
        return 0; //programa terminou como esperado
    }else{
        return 1;
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, char * token, char ** s){

    if(token != NULL && (strcmp(token, "CONST") == 0 || strcmp(token, "VAR") == 0 || strcmp(token, "PROCEDURE") == 0)){
        //Chama procedimento Declaracao
        p_declaracao(entrada, linha, i, token, s);
        token = obterSimbolo(entrada, linha, i);
    }
    
    if(token != NULL && (strcmp(token, IDENT) == 0 || strcmp(token, "CALL") == 0 || strcmp(token, "BEGIN") == 0 || strcmp(token, "IF") == 0 || strcmp(token, "WHILE") == 0)){
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s);
    }
    token = obterSimbolo(entrada, linha, i);

    return 0;
}

int p_declaracao(FILE * entrada, char * linha, int *i, char * token, char ** s){

    if(token != NULL && strcmp(token, "CONST") == 0){
        //Chama procedimento Constante
        p_contante(entrada, linha, i, token, s);
    }

    if(token != NULL && strcmp(token, "VAR") == 0){
        //Chama procedimento Variavel
        p_variavel(entrada, linha, i, token, s);
    }

    if(token != NULL && strcmp(token, "PROCEDURE") == 0){
        //Chama procedimento Procedimento
        p_procedimento(entrada, linha, i, token, s);
    }

    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, char * token, char ** s){
    
    token = obterSimbolo(entrada, linha, i);
    if(token != NULL && strcmp(token, IDENT) == 0){
        token = obterSimbolo(entrada, linha, i);
        if(token != NULL && strcmp(token, SIMB_IGUAL) == 0){
            token = obterSimbolo(entrada, linha, i);
            if(token != NULL && strcmp(token, SIMB_NUMERO) == 0){
                token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Mais_const
                p_mais_const(entrada, linha, i, token, s);
                if(token != NULL && strcmp(token, SIMB_PVIRGULA) == 0){
                    return 0;
                }
            }
        }
    }

    return 1; //erro
}

int p_mais_const(FILE * entrada, char * linha, int *i, char * token, char ** s){

    if(token != NULL && strcmp(token, SIMB_VIRGULA) == 0){
        token = obterSimbolo(entrada, linha, i);
        if(token != NULL && strcmp(token, IDENT) == 0){
            token = obterSimbolo(entrada, linha, i);
            if(token != NULL && strcmp(token, SIMB_IGUAL) == 0){
                token = obterSimbolo(entrada, linha, i);
                if(token != NULL && strcmp(token, SIMB_NUMERO) == 0){
                    token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Mais_const
                    p_mais_const(entrada, linha, i, token, s);
                }
            }
        }
    }

    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_mais_var(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_procedimento(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_comando(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_expressao(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_operador_unario(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_termo(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_mais_termos(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_fator(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_condicao(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}

int p_relacional(FILE * entrada, char * linha, int *i, char * token, char ** s){

    return 0;
}


