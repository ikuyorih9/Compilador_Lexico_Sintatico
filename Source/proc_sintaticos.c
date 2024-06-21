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

int p_erro(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    pprint("Iniciando tratamento de erro\n");
    int flag = 0;
    while(*token != NULL && flag == 0){
        for(int i = num_simb_sinc - 1; i>=0; i--){
            if(strcmp(*token, s[i]) == 0){
                pprint("Encontrou simbolo de sincronizacao: %s\n", s[i]);
                flag = 1;
                return 0;
            }else{
                pprint("Falha no tratamento: %s != %s\n", *token, s[i]);
            }
        }
        *token = obterSimbolo(entrada, linha, i);
    }
    pprint("Não encontrou simbolo de sincronizacao, chegou ao fim do programa :(\n");
    return 0;
}

int p_programa(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <programa>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    // O menor programa tem so um ponto final
    if(*token != NULL && strcmp(*token, SIMB_PONTO) == 0){
        pprint("Menor programa possível: somente um ponto.\n");
        return 0;
    }

    //Chama procedimento do Bloco
    p_bloco(entrada, linha, i, token, s, num_simb_sinc);

    //Depois de um bloco deve vir um ponto final
    pprint("Token: %s\n", *token);
    if(*token == NULL || strcmp(*token, SIMB_PONTO) == 0){ // por alguma razao nao ta retornando o token ponto
        pprint("Programa terminou como esperado (leu ponto final --> leu NULL)\n");
        return 0; //programa terminou como esperado
    }else{
        pprint("ERRO: esperava ponto final após o bloco\n");

        return 1;
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <bloco>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    
    if(*token != NULL && (strcmp(*token, "CONST") == 0 || strcmp(*token, "VAR") == 0 || strcmp(*token, "PROCEDURE") == 0)){
        //Chama procedimento Declaracao
        p_declaracao(entrada, linha, i, token, s, num_simb_sinc);
        //*token = obterSimbolo(entrada, linha, i);
    }
    
    if(*token != NULL && (strcmp(*token, IDENT) == 0 || strcmp(*token, "CALL") == 0 || strcmp(*token, "BEGIN") == 0 || strcmp(*token, "IF") == 0 || strcmp(*token, "WHILE") == 0)){
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
    }
    *token = obterSimbolo(entrada, linha, i);
    pprint("FINALIZANDO <bloco>.\n");
    pop();
    return 0;
}

int p_declaracao(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <declaracao>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, "CONST") == 0){
        //Chama procedimento Constante
        p_constante(entrada, linha, i, token, s, num_simb_sinc);
    }

    if(*token != NULL && strcmp(*token, "VAR") == 0){
        //Chama procedimento Variavel
        p_variavel(entrada, linha, i, token, s, num_simb_sinc);
    }

    if(*token != NULL && strcmp(*token, "PROCEDURE") == 0){
        //Chama procedimento Procedimento
        p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
    }
    pprint("FINALIZANDO <declaração>\n");
    pop();
    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <constante>.\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, SIMB_IGUAL) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_NUMERO) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Mais_const
                p_mais_const(entrada, linha, i, token, s, num_simb_sinc);
                if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    pprint("FINALIZANDO <constante>\n");
                    pop();
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
                    pprint("FINALIZANDO <constante>\n");
                    pop();
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
                pprint("FINALIZANDO <constante>\n");
                pop();
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
            pprint("FINALIZANDO <constante>\n");
            pop();
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
        pprint("FINALIZANDO <constante>\n");
        pop();
        return 0; //Foi capaz de corrigir o erro
    }
    pprint("ERRO: procedimento Constante.\n");
    return 1; //erro
}

int p_mais_const(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <mais_const>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_IGUAL) == 0){
                *token = obterSimbolo(entrada, linha, i);
                if(*token != NULL && strcmp(*token, SIMB_NUMERO) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Mais_const
                    p_mais_const(entrada, linha, i, token, s, num_simb_sinc);
                    pprint("FINALIZANDO <mais_const>\n");
                    pop();
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
                    pprint("FINALIZANDO <mais_const>\n");
                    pop();
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
                pprint("FINALIZANDO <mais_const>\n");
                pop();
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
            pprint("FINALIZANDO <mais_const>\n");
            pop();
            return 0; //Foi capaz de corrigir o erro
        }
    }{
        // OBS: se nao era SIMB_VIRGULA entao considera que gerou lambda
    }
    pprint("FINALIZANDO <mais_const>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <variavel>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    *token = obterSimbolo(entrada, linha, i);
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
            *token = obterSimbolo(entrada, linha, i);
            pprint("FINALIZANDO <variavel>\n");
            pop();
            return 0;
        }
    }
    pprint("ERRO: procedimento Variavel.\n");
    pprint("FINALIZANDO <variavel>\n");
    pop();
    return 1; //erro
}

int p_mais_var(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <mais_var>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, SIMB_VIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Mais_var
            p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
            pprint("FINALIZANDO <mais_var>\n");
            pop();
            return 0;
        }
    }
    pprint("FINALIZANDO <mais_var>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_procedimento(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <procedimento>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, "PROCEDURE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, IDENT) == 0){
            *token = obterSimbolo(entrada, linha, i);
            if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                *token = obterSimbolo(entrada, linha, i);
                //Chama procedimento Bloco
                p_bloco(entrada, linha, i, token, s, num_simb_sinc);
                if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
                    *token = obterSimbolo(entrada, linha, i);
                    //Chama procedimento Procedimento
                    p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
                    pprint("FINALIZANDO <procedimento>\n");
                    pop();
                    return 0; // sem erros
                }
            }
        }
    }
    pprint("FINALIZANDO <procedimento>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_comando(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <comando>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    //Se comando gera um atribuicao
    if(*token != NULL && strcmp(*token, IDENT) == 0){
        *token = obterSimbolo(entrada, linha, i);
        if(*token != NULL && strcmp(*token, SIMB_ATRIBUICAO) == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Expressao
            p_expressao(entrada, linha, i, token, s, num_simb_sinc);
            pprint("FINALIZANDO <comando>\n");
            pop();
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
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
        p_mais_cmd(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp(*token, "END") == 0){
            *token = obterSimbolo(entrada, linha, i);
            pprint("FINALIZANDO <comando>\n");
            pop();
            return 0; //sem erros
        }else{
            printf("ERRO: esperava END\n");
        }
    }else if(*token != NULL && strcmp(*token, "IF") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp(*token, "THEN") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pprint("FINALIZANDO <comando>\n");
            pop();
            return 0; //sem erros
        }
    }else if(*token != NULL && strcmp(*token, "WHILE") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp(*token, "DO") == 0){
            *token = obterSimbolo(entrada, linha, i);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pprint("FINALIZANDO <comando>\n");
            pop();
            return 0; //sem erros
        }
    }
    pprint("FINALIZANDO <comando>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <mais_cmd>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, SIMB_PVIRGULA) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais_cmd
        p_mais_cmd(entrada, linha, i, token, s, num_simb_sinc);
        pprint("FINALIZANDO <mais_cmd>\n");
        pop();
        return 0;
    }
    pprint("FINALIZANDO <mais_cmd>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_expressao(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <expressao>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    //Chama procedimento Operador unario
    p_operador_unario(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Termo
    p_termo(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Mais Termos
    p_mais_termos(entrada, linha, i, token, s, num_simb_sinc);
    pprint("FINALIZANDO <expressao>\n");
    pop();
    return 0;
}

int p_operador_unario(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <operador_unario>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    pprint("Token: %s\n", *token);
    if(*token != NULL && (strcmp(*token, SIMB_MENOS) == 0 || strcmp(*token, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pprint("FINALIZANDO <operador_unario>\n");
        pop();
        return 0; //sem erros
    }
    pprint("FINALIZANDO <operador_unario>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_termo(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <termo>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    //Chama procedimento Fator
    p_fator(entrada, linha, i, token, s, num_simb_sinc);
    //Chama procedimento Mais fatores
    p_mais_fatores(entrada, linha, i, token, s, num_simb_sinc);
    pprint("FINALIZANDO <termo>\n");
    pop();
    return 0;
}

int p_mais_termos(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <mais_termos>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && (strcmp(*token, SIMB_MENOS) == 0 || strcmp(*token, SIMB_MAIS) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Termo
        p_termo(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais Termos
        p_mais_termos(entrada, linha, i, token, s, num_simb_sinc);
        pprint("FINALIZANDO <mais_termos>\n");
        pop();
        return 0;
    }
    pprint("FINALIZANDO <mais_termos>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_fator(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <fator>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && (strcmp(*token, IDENT) == 0 || strcmp(*token, SIMB_NUMERO) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pprint("FINALIZANDO <fator>\n");
        pop();
        return 0;
    }else if(*token != NULL && strcmp(*token, SIMB_ABRE_PARENTESE) == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp(*token, SIMB_FECHA_PARENTESE) == 0){
            *token = obterSimbolo(entrada, linha, i);
            pprint("FINALIZANDO <fator>\n");
            pop();
            return 0; //sem erros
        }
    }
    pprint("FINALIZANDO <fator>\n");
    pop();
    return 1;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <mais_fatores>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && (strcmp(*token, SIMB_MULTI) == 0 || strcmp(*token, SIMB_DIV) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Fator
        p_fator(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Mais Fatores
        p_mais_fatores(entrada, linha, i, token, s, num_simb_sinc);
        pprint("FINALIZANDO <mais_fatores>\n");
        pop();
        return 0; // sem erros
    }
    pprint("FINALIZANDO <mais_fatores>\n");
    pop();
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_condicao(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <condicao>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && strcmp(*token, "ODD") == 0){
        *token = obterSimbolo(entrada, linha, i);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        pprint("FINALIZANDO <condicao>: ODD\n");
        pop();
        return 0; //sem erros
    }else{
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Relacional
        p_relacional(entrada, linha, i, token, s, num_simb_sinc);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        pprint("FINALIZANDO <condicao>\n");
        pop();
        return 0; //sem erros
    }
    pprint("FINALIZANDO <condicao>\n");
    pop();
    return 1; //erro
}

int p_relacional(FILE * entrada, char * linha, int *i, char ** token, char ** s, int num_simb_sinc){
    push();
    pprint("INICIANDO <relacional>\n");
    pprint("Procedimento inicia com TOKEN: %s\n", *token);
    if(*token != NULL && 
        (strcmp(*token, SIMB_IGUAL) == 0 || strcmp(*token, SIMB_DIFERENTE) == 0 ||
         strcmp(*token, SIMB_MENOR) == 0 || strcmp(*token, SIMB_MENOR_IGUAL) == 0 ||
         strcmp(*token, SIMB_MAIOR) == 0 || strcmp(*token, SIMB_MAIOR_IGUAL) == 0)){
        *token = obterSimbolo(entrada, linha, i);
        pprint("FINALIZANDO <relacional>\n");
        pop();
        return 0; //sem erros
    }
    pprint("FINALIZANDO <relacional>\n");
    pop();
    return 1; //erro 
}


