#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_sintaticos.h"
#include "analisador_lexico.h"
#include "simbolos.h"
#include "tabelas.h"

#define pprint for(int i = 0; i < contTab; i++) printf("\t"); printf

int contTab = 0;
void tab(){
    for(int i = 0; i < contTab; i++)
        printf("\t");
}

void push(){
    contTab++;
}

void pop(){
    contTab--;
}

/*
Procedimento de erro.
@param entrada arquivo de entrada contendo o código-fonte.
@param cadeia de símbolos de uma linha da entrada.
@param i iterador da cadeia de símbolos.
@param token token recebido em busca dos erros.
@param s tabela de erros.
*/
int p_erro(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("Iniciando tratamento de erro\n");
    int flag = 0;
    while(*token != NULL && flag == 0){
        for(int i = num_simb_sinc - 1; i>=0; i--){
            if(strcmp((*token)->tipo, s[i]) == 0){
                pprint("Encontrou simbolo de sincronizacao: %s\n", s[i]);
                flag = 1;
                return 0;
            }else{
                pprint("Falha no tratamento: %s != %s\n", (*token)->tipo, s[i]);
            }
        }
        *token = obterSimbolo(entrada, linha, i);
    }
    pprint("Não encontrou simbolo de sincronizacao, chegou ao fim do programa :(\n");
    return 0;
}

int p_programa(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <programa>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    // O menor programa tem so um ponto final
    if(*token != NULL && strcmp((*token)->tipo, SIMB_PONTO) == 0){
        pprint("Menor programa possível: somente um ponto.\n");
        return 0;
    }

    //Chama procedimento do Bloco
    p_bloco(entrada, linha, i, token, s, num_simb_sinc);

    //Depois de um bloco deve vir um ponto final
    pprint("Token: %s\n", (*token)->tipo);
    if(*token == NULL || strcmp((*token)->tipo, SIMB_PONTO) == 0){ // por alguma razao nao ta retornando o token ponto
        pprint("Programa terminou como esperado (leu ponto final --> leu NULL)\n");
        pop();
        pprint("FINALIZANDO <bloco>.\n");
        return 0; //programa terminou como esperado
    }else{
        pprint("ERRO: esperava ponto final após o bloco\n");
        pop();
        pprint("FINALIZANDO <bloco>.\n");
        return 1;
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <bloco>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    
    if(*token != NULL && (strcmp((*token)->tipo, "CONST") == 0 || strcmp((*token)->tipo, "VAR") == 0 || strcmp((*token)->tipo, "PROCEDURE") == 0)){
        //Chama procedimento Declaracao
        p_declaracao(entrada, linha, i, token, s, num_simb_sinc);
    }
    
    if(*token != NULL && (strcmp((*token)->tipo, IDENT) == 0 || strcmp((*token)->tipo, "CALL") == 0 || strcmp((*token)->tipo, "BEGIN") == 0 || strcmp((*token)->tipo, "IF") == 0 || strcmp((*token)->tipo, "WHILE") == 0)){
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
    }
    *token = obterSimbolo(entrada, linha, i);
    pop();
    pprint("FINALIZANDO <bloco>.\n");
    return 0;
}

int p_declaracao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <declaracao>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "CONST") == 0){
        //Chama procedimento Constante
        p_constante(entrada, linha, i, token, s, num_simb_sinc);
    }

    if(*token != NULL && strcmp((*token)->tipo, "VAR") == 0){
        //Chama procedimento Variavel
        p_variavel(entrada, linha, i, token, s, num_simb_sinc);
    }

    if(*token != NULL && strcmp((*token)->tipo, "PROCEDURE") == 0){
        //Chama procedimento Procedimento
        p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
    }
    pop();
    pprint("FINALIZANDO <declaração>\n");
    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <constante>.\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_IGUAL) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp((*token)->tipo, SIMB_NUMERO) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Mais_const
                p_mais_const(entrada, linha, i, token, s, num_simb_sinc);
                if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    pop();
                    pprint("FINALIZANDO <constante>\n");
                    return 0;
                }
                else{
                    // ERRO: esperava SIMB_PVIRGULA (;)
                    s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
                    s[num_simb_sinc] = malloc(sizeof(char)*30);
                    strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                    pprint("Sincronizacao 1: %s\n", s[num_simb_sinc]);
                    printf("Erro sintático: SIMB_PVIRGULA (;) faltando na posição %d da linha %s", *i, linha);
                    p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
                    *token = obterSimbolo(entrada, linha, i);
                    pop();
                    pprint("FINALIZANDO <constante>\n");
                    return 0; //Foi capaz de corrigir o erro
                }
            }
            else{
                // ERRO: esperava SIMB_NUMERO
                s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
                s[num_simb_sinc] = malloc(sizeof(char)*30);
                s[num_simb_sinc + 1] = malloc(sizeof(char)*30);
                strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                strcpy(s[num_simb_sinc + 1], SIMB_NUMERO);
                pprint("Sincronizacao 2: %s\n", s[num_simb_sinc]);
                printf("Erro sintático: SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
                *token = obterSimbolo(entrada, linha, i);
                pop();
                pprint("FINALIZANDO <constante>\n");
                return 0; //Foi capaz de corrigir o erro
            }
        }
        else{
            // ERRO: esperava SIMB_IGUAL
            s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
            s[num_simb_sinc] = malloc(sizeof(char)*30);
            s[num_simb_sinc + 1] = malloc(sizeof(char)*30);
            strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
            strcpy(s[num_simb_sinc + 1], SIMB_IGUAL);
            pprint("Sincronizacao 2: %s\n", s[num_simb_sinc]);
            printf("Erro sintático: SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
            *token = obterSimbolo(entrada, linha, i);
            pop();
            pprint("FINALIZANDO <constante>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }{
        // ERRO: esperava IDENT
        s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
        s[num_simb_sinc] = malloc(sizeof(char)*30);
        strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
        pprint("Sincronizacao 1: %s\n", s[num_simb_sinc]);
        printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
        *token = obterSimbolo(entrada, linha, i);
        pop();
        pprint("FINALIZANDO <constante>\n");
        return 0; //Foi capaz de corrigir o erro
    }
    pprint("ERRO: procedimento Constante.\n");
    return 1; //erro
}

int p_mais_const(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_const>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp((*token)->tipo, SIMB_IGUAL) == 0){
                *token = obterSimbolo(entrada, linha, i);
                if(*token != NULL && strcmp((*token)->tipo, SIMB_NUMERO) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Mais_const
                    p_mais_const(entrada, linha, i, token, s, num_simb_sinc);
                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0;
                }{
                    // ERRO: esperava SIMB_NUMERO
                    s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
                    s[num_simb_sinc] = malloc(sizeof(char)*30);
                    s[num_simb_sinc + 1] = malloc(sizeof(char)*30);
                    strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                    strcpy(s[num_simb_sinc + 1], SIMB_NUMERO);
                    pprint("Sincronizacao 2: %s\n", s[num_simb_sinc]);
                    printf("Erro sintático: SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                    p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
                    *token = obterSimbolo(entrada, linha, i);
                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0; //Foi capaz de corrigir o erro
                }
            }{
                // ERRO: esperava SIMB_IGUAL
                s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
                s[num_simb_sinc] = malloc(sizeof(char)*30);
                s[num_simb_sinc + 1] = malloc(sizeof(char)*30);
                strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                strcpy(s[num_simb_sinc + 1], SIMB_IGUAL);
                pprint("Sincronizacao 2: %s\n", s[num_simb_sinc]);
                printf("Erro sintático: SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
                p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
                *token = obterSimbolo(entrada, linha, i);
                pop();
                pprint("FINALIZANDO <mais_const>\n");
                return 0; //Foi capaz de corrigir o erro
            }
        }{
            // ERRO: esperava IDENT
            s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
            s[num_simb_sinc] = malloc(sizeof(char)*30);
            strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
            pprint("Sincronizacao 1: %s\n", s[num_simb_sinc]);
            printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
            *token = obterSimbolo(entrada, linha, i);
            pop();
            pprint("FINALIZANDO <mais_const>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }{
        // OBS: se nao era SIMB_VIRGULA entao considera que gerou lambda
    }
    pop();
    pprint("FINALIZANDO <mais_const>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <variavel>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
            *token = obterSimbolo(entrada, linha, i);
            pop();
            pprint("FINALIZANDO <variavel>\n");
            return 0;
        }
    }
    pprint("ERRO: procedimento Variavel.\n");
    pop();
    pprint("FINALIZANDO <variavel>\n");
    return 1; //erro
}

int p_mais_var(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_var>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Mais_var
            p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <mais_var>\n");
            return 0;
        }
    }
    pop();
    pprint("FINALIZANDO <mais_var>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_procedimento(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <procedimento>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "PROCEDURE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Bloco
                p_bloco(entrada, linha, i, token, s, num_simb_sinc);
                if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Procedimento
                    p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
                    pop();
                    pprint("FINALIZANDO <procedimento>\n");
                    return 0; // sem erros
                }
            }
        }
    }
    pop();
    pprint("FINALIZANDO <procedimento>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_comando(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <comando>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    //Se comando gera um atribuicao
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_ATRIBUICAO) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Expressao
            p_expressao(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 0;
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "CALL") == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            return 0;
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "BEGIN") == 0){
        *token = obterSimbolo(entrada, linha, i);
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
        p_mais_cmd(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "END") == 0){
            *token = obterSimbolo(entrada, linha, i);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 0; //sem erros
        }else{
            printf("ERRO: esperava END\n");
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "IF") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "THEN") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 0; //sem erros
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "WHILE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "DO") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 0; //sem erros
        }
    }
    pop();
    pprint("FINALIZANDO <comando>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_cmd>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais_cmd
        p_mais_cmd(entrada, linha, i, token, s, num_simb_sinc);
        pop();
        pprint("FINALIZANDO <mais_cmd>\n");
        return 0;
    }
    pop();
    pprint("FINALIZANDO <mais_cmd>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_expressao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <expressao>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    //Chama procedimento Operador unario
    p_operador_unario(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Termo
    p_termo(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Mais Termos
    p_mais_termos(entrada, linha, i, token, s, num_simb_sinc);
    pop();
    pprint("FINALIZANDO <expressao>\n");
    return 0;
}

int p_operador_unario(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <operador_unario>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    pprint("Token: %s\n", (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MENOS) == 0 || strcmp((*token)->tipo, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pop();
        pprint("FINALIZANDO <operador_unario>\n");
        return 0; //sem erros
    }
    pop();
    pprint("FINALIZANDO <operador_unario>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_termo(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <termo>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    //Chama procedimento Fator
    p_fator(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Mais fatores
    p_mais_fatores(entrada, linha, i, token, s, num_simb_sinc);
    pop();
    pprint("FINALIZANDO <termo>\n");
    return 0;
}

int p_mais_termos(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_termos>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MENOS) == 0 || strcmp((*token)->tipo, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Termo
        p_termo(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais Termos
        p_mais_termos(entrada, linha, i, token, s, num_simb_sinc);
        pop();
        pprint("FINALIZANDO <mais_termos>\n");
        return 0;
    }
    pop();
    pprint("FINALIZANDO <mais_termos>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_fator(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <fator>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, IDENT) == 0 || strcmp((*token)->tipo, SIMB_NUMERO) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pop();
        pprint("FINALIZANDO <fator>\n");
        return 0;
    }else if(*token != NULL && strcmp((*token)->tipo, SIMB_ABRE_PARENTESE) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_FECHA_PARENTESE) == 0){
            *token = obterSimbolo(entrada, linha, i);
            pop();
            pprint("FINALIZANDO <fator>\n");
            return 0; //sem erros
        }
    }
    pop();
    pprint("FINALIZANDO <fator>\n");
    return 1;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_fatores>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MULTI) == 0 || strcmp((*token)->tipo, SIMB_DIV) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Fator
        p_fator(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais Fatores
        p_mais_fatores(entrada, linha, i, token, s, num_simb_sinc);
        pop();
        pprint("FINALIZANDO <mais_fatores>\n");
        return 0; // sem erros
    }
    pop();
    pprint("FINALIZANDO <mais_fatores>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_condicao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <condicao>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "ODD") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        pop();
        pprint("FINALIZANDO <condicao>: ODD\n");
        return 0; //sem erros
    }else{
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Relacional
        p_relacional(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        pop();
        pprint("FINALIZANDO <condicao>\n");
        return 0; //sem erros
    }
    pop();
    pprint("FINALIZANDO <condicao>\n");
    return 1; //erro
}

int p_relacional(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <relacional>\n");
    push();
    pprint("Procedimento inicia com TOKEN: %s\n", (*token)->tipo);
    if(*token != NULL && 
        (strcmp((*token)->tipo, SIMB_IGUAL) == 0 || strcmp((*token)->tipo, SIMB_DIFERENTE) == 0 ||
         strcmp((*token)->tipo, SIMB_MENOR) == 0 || strcmp((*token)->tipo, SIMB_MENOR_IGUAL) == 0 ||
         strcmp((*token)->tipo, SIMB_MAIOR) == 0 || strcmp((*token)->tipo, SIMB_MAIOR_IGUAL) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pop();
        pprint("FINALIZANDO <relacional>\n");
        return 0; //sem erros
    }
    pop();
    pprint("FINALIZANDO <relacional>\n");
    return 1; //erro 
}


