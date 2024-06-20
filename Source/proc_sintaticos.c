#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_sintaticos.h"
#include "analisador_lexico.h"
#include "simbolos.h"
#include "tabelas.h"

int p_programa(FILE * entrada, char * linha, int *i, char **token, char ** s){
    // O menor programa tem so um ponto final
    if(*token != NULL && strcmp(*token, SIMB_PONTO) == 0){
        dprint("Menor programa possível: somente um ponto.\n");
        return 0;
    }

    //Chama procedimento do Bloco
    p_bloco(entrada, linha, i, token, s);

    //Depois de um bloco deve vir um ponto final
    if(*token == NULL || strcmp(*token, SIMB_PONTO) == 0){ // por alguma razao nao ta retornando o token ponto
        dprint("Programa terminou como esperado (leu ponto final --> leu NULL)\n");
        return 0; //programa terminou como esperado
    }else{
        dprint("ERRO: esperava ponto final após o bloco\n");
        return 1;
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Bloco.\n");
    
    if(*token != NULL && (strcmp(*token, "CONST") == 0 || strcmp(*token, "VAR") == 0 || strcmp(*token, "PROCEDURE") == 0)){
        //Chama procedimento Declaracao
        p_declaracao(entrada, linha, i, token, s);
        //*token = obterSimbolo(entrada, linha, i);
    }
    
    if(*token != NULL && (strcmp(*token, IDENT) == 0 || strcmp(*token, "CALL") == 0 || strcmp(*token, "BEGIN") == 0 || strcmp(*token, "IF") == 0 || strcmp(*token, "WHILE") == 0)){
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s);
    }
    *token = obterSimbolo(entrada, linha, i);
    dprint("Fim procedimento Bloco.\n");
    return 0;
}

int p_declaracao(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Declaracao.\n");
    if(*token != NULL && strcmp(*token, "CONST") == 0){
        //Chama procedimento Constante
        p_constante(entrada, linha, i, token, s);
    }

    if(*token != NULL && strcmp(*token, "VAR") == 0){
        //Chama procedimento Variavel
        p_variavel(entrada, linha, i, token, s);
    }

    if(*token != NULL && strcmp(*token, "PROCEDURE") == 0){
        //Chama procedimento Procedimento
        p_procedimento(entrada, linha, i, token, s);
    }
    dprint("Fim procedimento Declaração.\n");
    dprint("Token apos declaracao: %s\n", *token);
    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Constante.\n");
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, SIMB_IGUAL) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_NUMERO) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Mais_const
                p_mais_const(entrada, linha, i, token, s);
                if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                    dprint("Fim procedimento Constante.\n");
                    return 0;
                }
            }
        }
    }
    dprint("ERRO: procedimento Constante.\n");
    return 1; //erro
}

int p_mais_const(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Mais Constante.\n");
    if(*token != NULL && strcmp(*token, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_IGUAL) == 0){
                *token = obterSimbolo(entrada, linha, i);
                if(*token != NULL && strcmp(*token, SIMB_NUMERO) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Mais_const
                    p_mais_const(entrada, linha, i, token, s);
                    dprint("Fim procedimento Mais Constante (1).\n");
                    return 0;
                }
            }
        }
    }
    dprint("Fim procedimento Mais Constante (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Variavel.\n");
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Mais_var
        dprint("*token antes mais_var: %s.\n", *token);
        p_mais_var(entrada, linha, i, token, s);
        dprint("*token apos mais_var: %s.\n", *token);
        if(*token != NULL && (strcmp(*token, SIMB_VIRGULA) == 0 || strcmp(*token, SIMB_PVIRGULA) == 0)){ //Tem um problema aqui que nao consegui resolver: mais var ta reotrnando virgula ao inver de pvirgula na chamada recursiva !!
            *token = obterSimbolo(entrada, linha, i);
            dprint("Fim procedimento Variavel.\n");
            return 0;
        }
    }
    dprint("ERRO: procedimento Variavel.\n");
    return 1; //erro
}

int p_mais_var(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Mais Variavel.\n");
    if(*token != NULL && strcmp(*token, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Mais_var
            p_mais_var(entrada, linha, i, token, s);
            dprint("*token ap mais_var: %s.\n", *token);
            dprint("Fim procedimento Mais Variavel (1).\n");
            return 0;
        }
    }
    dprint("*token mais_var: %s.\n", *token);
    dprint("Fim procedimento Mais Variavel (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_procedimento(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Procedimento.\n");
    if(*token != NULL && strcmp(*token, "PROCEDURE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Bloco
                p_bloco(entrada, linha, i, token, s);
                if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Procedimento
                    p_procedimento(entrada, linha, i, token, s);
                    dprint("Fim procedimento Procedimento (1).\n");
                    return 0; // sem erros
                }
            }
        }
    }
    dprint("Fim procedimento Procedimento (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_comando(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Comando.\n");
    //Se comando gera um atribuicao
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, SIMB_ATRIBUICAO) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Expressao
            p_expressao(entrada, linha, i, token, s);
            dprint("*token apos expressao %s\n", *token);
            return 0;
        }
    } else if(*token != NULL && strcmp(*token, "CALL") == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            return 0;
        }
    } else if(*token != NULL && strcmp(*token, "BEGIN") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s);
        dprint("Token apos cmd: %s\n",*token);
        //*token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Mais_cmd
        p_mais_cmd(entrada, linha, i, token, s);
        dprint("Token apos mais cmds: %s\n",*token);
        if(*token != NULL && strcmp(*token, "END") == 0){
            *token = obterSimbolo(entrada, linha, i);
            dprint("Token apos mais cmds 2: %s\n",*token);
            return 0; //sem erros
        }else{
            printf("ERRO: esperava END\n");
        }
    }else if(*token != NULL && strcmp(*token, "IF") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s);
        if(*token != NULL && strcmp(*token, "THEN") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s);
            return 0; //sem erros
        }
    }else if(*token != NULL && strcmp(*token, "WHILE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s);
        if(*token != NULL && strcmp(*token, "DO") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s);
            return 0; //sem erros
        }
    }
    dprint("Fim procedimento Comando.\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Mais Comandos.\n");
    if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s);
        //Chama procedimento Mais_cmd
        p_mais_cmd(entrada, linha, i, token, s);
        dprint("Fim procedimento Mais Comandos (1).\n");
        return 0;
    }
    dprint("Fim procedimento Mais Comandos (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_expressao(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Expressao.\n");
    //Chama procedimento Operador unario
    p_operador_unario(entrada, linha, i, token, s);
    //Chama procedimento Termo
    p_termo(entrada, linha, i, token, s);
    //Chama procedimento Mais Termos
    p_mais_termos(entrada, linha, i, token, s);
    dprint("*token: %s\n", *token);
    dprint("Fim procedimento Expressao.\n");
    return 0;
}

int p_operador_unario(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Operador Unario.\n");
    if(*token != NULL && (strcmp(*token, SIMB_MENOS) == 0 || strcmp(*token, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        dprint("Fim procedimento Operador Unario (1).\n");
        return 0; //sem erros
    }
    dprint("Fim procedimento Operador Unario (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_termo(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Termo.\n");
    //Chama procedimento Fator
    p_fator(entrada, linha, i, token, s);
    //Chama procedimento Mais fatores
    p_mais_fatores(entrada, linha, i, token, s);
    dprint("Fim procedimento Termo.\n");
    return 0;
}

int p_mais_termos(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Mais Termos.\n");
    if(*token != NULL && (strcmp(*token, SIMB_MENOS) == 0 || strcmp(*token, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Termo
        p_termo(entrada, linha, i, token, s);
        //Chama procedimento Mais Termos
        p_mais_termos(entrada, linha, i, token, s);
        dprint("Fim procedimento Mais Termos (1).\n");
        return 0;
    }
    dprint("Fim procedimento Mais Termos (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_fator(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Fator.\n");
    if(*token != NULL && (strcmp(*token, IDENT) == 0 || strcmp(*token, SIMB_NUMERO) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        dprint("Fim procedimento Fator: ident ou numero.\n");
        return 0;
    }else if(*token != NULL && strcmp(*token, SIMB_ABRE_PARENTESE) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s);
        if(*token != NULL && strcmp(*token, SIMB_FECHA_PARENTESE) == 0){
            *token = obterSimbolo(entrada, linha, i);
            dprint("Fim procedimento Fator: (expressao).\n");
            return 0; //sem erros
        }
    }
    dprint("Fim procedimento Fator (2).\n");
    return 1;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Mais Fatores.\n");
    if(*token != NULL && (strcmp(*token, SIMB_MULTI) == 0 || strcmp(*token, SIMB_DIV) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Fator
        p_fator(entrada, linha, i, token, s);
        //Chama procedimento Mais Fatores
        p_mais_fatores(entrada, linha, i, token, s);
        dprint("Fim procedimento Mais Fatores (1).\n");
        return 0; // sem erros
    }
    dprint("Fim procedimento Mais Fatores (2).\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_condicao(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Condicao.\n");
    if(*token != NULL && strcmp(*token, "ODD")){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s);
        dprint("Fim procedimento Condicao: ODD.\n");
        return 0; //sem erros
    }else{
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s);
        //Chama procedimento Relacional
        p_relacional(entrada, linha, i, token, s);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s);
        dprint("Fim procedimento Condicao: expressao relacional.\n");
        return 0; //sem erros
    }
    dprint("Fim procedimento Condicao.\n");
    return 1; //erro
}

int p_relacional(FILE * entrada, char * linha, int *i, char **token, char ** s){
    dprint("Executando procedimento Relacional.\n");
    if(*token != NULL && 
        (strcmp(*token, SIMB_IGUAL) == 0 || strcmp(*token, SIMB_DIFERENTE) == 0 ||
         strcmp(*token, SIMB_MENOR) == 0 || strcmp(*token, SIMB_MENOR_IGUAL) == 0 ||
         strcmp(*token, SIMB_MAIOR) == 0 || strcmp(*token, SIMB_MAIOR_IGUAL) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        dprint("Fim procedimento Relacional.\n");
        return 0; //sem erros
    }
    dprint("ERRO: procedimento Relacional.\n");
    return 1; //erro 
}


