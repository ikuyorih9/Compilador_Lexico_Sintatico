#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_sintaticos.h"
#include "analisador_lexico.h"
#include "simbolos.h"
#include "tabelas.h"
#include "token.h"

#define PPRINT_ACTV 1
#define pprint for(int i = 0; PPRINT_ACTV && i < contTab; i++) printf("\t"); if(PPRINT_ACTV) printf

int contTab = 0;

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

    // Exibicao de erros lexicos
    if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
        printf("Erro Lexico: %s\n", (*token)->valor);
    }

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
        //ESSE OBTER SIMBOLO É UM PROBLEMA QUANDO ENCONTRA SYNC. RODAR O CODIGO PARA VER DOIS ERROS SINTATICOS.
        obterSimbolo(entrada, linha, i, token);
    }
    pprint("Não encontrou simbolo de sincronizacao, chegou ao fim do programa :(\n");
    return 0;
}

int p_programa(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <programa>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    //Detecção do menor programa possível.
    if(*token != NULL && strcmp((*token)->tipo, SIMB_PONTO) == 0)
        return 0;

    p_bloco(entrada, linha, i, token, s, num_simb_sinc);

    //Depois de um bloco deve vir um ponto final
    if(*token == NULL || strcmp((*token)->tipo, SIMB_PONTO) == 0){
        pprint("Programa terminou como esperado (leu ponto final --> leu NULL)\n");
        pop();
        pprint("FINALIZANDO <bloco>.\n");
        return 0; //programa terminou como esperado
    }else{//Erro 
        s = (char**) realloc(s, sizeof(char *) * (num_simb_sinc + 1));
        s[num_simb_sinc] = malloc(sizeof(char) * TAM_SIMBOLO);
        strcpy(s[num_simb_sinc], SIMB_PONTO);
        printf("ERRO SINTÁTICO: esperava SIMB_PONTO na posição %d da linha %s\n", *i, linha);
        p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
        obterSimbolo(entrada, linha, i, token);
        pop();
        pprint("FINALIZANDO <bloco> 2\n");
        return 1;
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <bloco>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    
    if(*token != NULL){
        //Verifica se o símbolo é uma declaração.
        int flag = 0;
        if(cmpToken(*token, "CONST")|| 
           cmpToken(*token, "VAR") || 
           cmpToken(*token, "PROCEDURE")){
            p_declaracao(entrada, linha, i, token, s, num_simb_sinc);
            flag = 1;
        } 
        
        if(cmpToken(*token, IDENT) || cmpToken(*token, "CALL")|| 
           cmpToken(*token, "BEGIN") || cmpToken(*token, "IF")|| 
           cmpToken(*token, "WHILE")){ //Verifica se o símbolo é um comando.
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            flag = 1;
        } 
        
        if(flag == 0){//Erro 
            //Adiciona o símbolo na fila de erros.
            s = (char**) realloc(s, sizeof(char *) * (num_simb_sinc + 1));
            s[num_simb_sinc] = malloc(sizeof(char) * TAM_SIMBOLO);
            strcpy(s[num_simb_sinc], SIMB_PONTO);
            printf("ERRO SINTÁTICO: esperava CONST, VAR, PROCEDURE, BEGIN, CALL, IF, WHILE ou IDENT na posição %d da linha %s\n", *i, linha);
            p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <bloco>\n");
            return 0;
        }
    }

    //Obtém o próximo símbolo
    //obterSimbolo(entrada, linha, i, token);

    pop();
    pprint("FINALIZANDO <bloco>.\n");
    pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 0;
}

int p_declaracao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <declaracao>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    if(*token != NULL){
        if(cmpToken(*token, "CONST")){
            p_constante(entrada, linha, i, token, s, num_simb_sinc);
        }   
        if(cmpToken(*token, "VAR")){
            p_variavel(entrada, linha, i, token, s, num_simb_sinc);
        }           
        if(cmpToken(*token, "PROCEDURE")){
            p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
        }      
    }

    pop();
    pprint("FINALIZANDO <declaração>\n");
    pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <constante>.\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    obterSimbolo(entrada, linha, i, token);
    pprint("Novo símbolo obtido: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    //Se o símbolo é um identificador.
    if(*token != NULL && cmpToken(*token, IDENT)){
        obterSimbolo(entrada, linha, i, token);
        pprint("IDENT encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

        //Se o símbolo é uma igualdade (=).
        if(*token != NULL && cmpToken(*token, SIMB_IGUAL)){
            obterSimbolo(entrada, linha, i, token);
            pprint("SIMB_IGUAL (=) encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

            //Se o símbolo é um número.
            if(*token != NULL && cmpToken(*token, SIMB_NUMERO)){
                obterSimbolo(entrada, linha, i, token);
                pprint("SIMB_NUMERO encontrado!\n");
                pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

                p_mais_const(entrada, linha, i, token, s, num_simb_sinc);

                //Se o símbolo é um ponto-e-vírgula (;).
                if(*token != NULL && cmpToken(*token, SIMB_PVIRGULA)){
                    obterSimbolo(entrada, linha, i, token);
                    pprint("SIMB_PVIRGULA (;) encontrado!\n");
                    pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
                    pop();
                    pprint("FINALIZANDO <constante>\n");
                    return 0;
                }
                //Erro ao encontrar um SIMB_PVIRGULA.
                else{
                    //Adiciona o símbolo na fila de erros.
                    s = (char**) realloc(s, sizeof(char *) * (num_simb_sinc + 1));
                    s[num_simb_sinc] = malloc(sizeof(char) * TAM_SIMBOLO);
                    strcpy(s[num_simb_sinc], SIMB_PVIRGULA);

                    printf("ERRO SINTÁTICO: SIMB_PVIRGULA (;) faltando na posição %d da linha %s\n", *i, linha);
                    p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
                    obterSimbolo(entrada, linha, i, token);
                    pop();
                    pprint("FINALIZANDO <constante>\n");
                    return 0;
                }
            }
            //Erro ao encontrar um NUMERO.
            else{
                //Adiciona o símbolo na fila de erros.
                s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
                s[num_simb_sinc] = malloc(sizeof(char)*TAM_SIMBOLO);
                s[num_simb_sinc + 1] = malloc(sizeof(char)*TAM_SIMBOLO);
                strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                strcpy(s[num_simb_sinc + 1], SIMB_NUMERO);

                printf("Erro sintático: SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
                obterSimbolo(entrada, linha, i, token);

                pop();
                pprint("FINALIZANDO <constante>\n");
                return 0;
            }
        }
        //Erro ao encontrar SIMB_IGUAL.
        else{
            //Adiciona o símbolo na fila de erros.
            s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
            s[num_simb_sinc] = malloc(sizeof(char)*30);
            s[num_simb_sinc + 1] = malloc(sizeof(char)*30);
            strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
            strcpy(s[num_simb_sinc + 1], SIMB_IGUAL);
            
            printf("Erro sintático: SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
            obterSimbolo(entrada, linha, i, token);

            pop();
            pprint("FINALIZANDO <constante>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }
    //Erro ao encontrar um IDENT.
    else{
        //Adiciona o símbolo na fila de erros.
        s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
        s[num_simb_sinc] = malloc(sizeof(char)*30);
        strcpy(s[num_simb_sinc], SIMB_PVIRGULA);

        printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
        obterSimbolo(entrada, linha, i, token);

        pop();
        pprint("FINALIZANDO <constante>\n");
        return 0; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("ERRO: FINALIZANDO <constante>\n");
    return 1;
}

int p_mais_const(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_const>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    //Se o símbolo é SIMB_VIRGULA.
    if(*token != NULL && cmpToken(*token, SIMB_VIRGULA)){
        obterSimbolo(entrada, linha, i, token);
        pprint("SIMB_VIRGULA encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

        //Se o símbolo é IDENT.
        if(*token != NULL && cmpToken(*token, IDENT)){
            obterSimbolo(entrada, linha, i, token);
            pprint("IDENT encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

            //Se o símbolo é SIMB_IGUAL.
            if(*token != NULL && cmpToken(*token, SIMB_IGUAL)){
                obterSimbolo(entrada, linha, i, token);
                pprint("SIMB_IGUAL encontrado!\n");
                pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

                //Se o símbolo é um SIMB_NUMERO.
                if(*token != NULL && cmpToken(*token, SIMB_NUMERO)){
                    obterSimbolo(entrada, linha, i, token);
                    pprint("SIMB_IGUAL encontrado!\n");
                    pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

                    //Chama procedimento <mais_const>
                    p_mais_const(entrada, linha, i, token, s, num_simb_sinc);

                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0;
                }
                //Erro ao encontrar um SIMB_NUMERO.
                else{
                    //Adiciona o símbolo na fila de erros.
                    s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 2));
                    s[num_simb_sinc] = malloc(sizeof(char) * TAM_SIMBOLO);
                    s[num_simb_sinc + 1] = malloc(sizeof(char) * TAM_SIMBOLO);
                    strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                    strcpy(s[num_simb_sinc + 1], SIMB_NUMERO);

                    printf("Erro sintático: SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                    p_erro(entrada, linha, i, token, s, num_simb_sinc + 2);
                    //obterSimbolo(entrada, linha, i, token);

                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0; //Foi capaz de corrigir o erro
                }
            }
            //Erro ao encontrar um SIMB_IGUAL.
            else{
                //Adiciona o símbolo na fila de erros.
                s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
                s[num_simb_sinc] = malloc(sizeof(char)*TAM_SIMBOLO);
                //s[num_simb_sinc + 1] = malloc(sizeof(char)*TAM_SIMBOLO);
                strcpy(s[num_simb_sinc], SIMB_PVIRGULA);
                //strcpy(s[num_simb_sinc + 1], SIMB_IGUAL);

                
                printf("Erro sintático: SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
                p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
                obterSimbolo(entrada, linha, i, token);

                pop();
                pprint("FINALIZANDO <mais_const>\n");
                return 0; //Foi capaz de corrigir o erro
            }
        }
        //Erro ao encontrar um IDENT.
        else{
            //Adiciona o símbolo na fila de erros.
            s = (char**) realloc(s,sizeof(char *)*(num_simb_sinc + 1));
            s[num_simb_sinc] = malloc(sizeof(char)*TAM_SIMBOLO);
            strcpy(s[num_simb_sinc], SIMB_PVIRGULA);

            printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s, num_simb_sinc + 1);
            //obterSimbolo(entrada, linha, i, token);

            pop();
            pprint("FINALIZANDO <mais_const>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }

    pop();
    pprint("FINALIZANDO <mais_const>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <variavel>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    obterSimbolo(entrada, linha, i, token);
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        obterSimbolo(entrada, linha, i, token);
        p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <variavel>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0;
        }//Erro ao encontrar um SIMB_PVIRGULA.
        else{
            //Adiciona o símbolo na fila de erros.
            char ** s1 = (char**) malloc(sizeof(char *) *7);
            for(int i = 0; i< 7; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[6], "PROCEDURE");
            strcpy(s1[5], "CALL");
            strcpy(s1[4], "BEGIN");
            strcpy(s1[3], "IF");
            strcpy(s1[2], "WHILE");
            strcpy(s1[1], SIMB_PVIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 7);
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <variavel>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }//Erro ao encontrar um IDENT.
    else{
        char ** s1 = (char**) malloc(sizeof(char *) *7);
        for(int i = 0; i< 7; i++){
            s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
        }
        strcpy(s1[6], "PROCEDURE");
        strcpy(s1[5], "CALL");
        strcpy(s1[4], "BEGIN");
        strcpy(s1[3], "IF");
        strcpy(s1[2], "WHILE");
        strcpy(s1[1], SIMB_PVIRGULA);
        strcpy(s1[0], SIMB_PONTO);
        printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s1, 7);
        obterSimbolo(entrada, linha, i, token);
        pop();
        pprint("FINALIZANDO <variavel>\n");
        return 1; //Foi capaz de corrigir o erro
    }
    pprint("ERRO: procedimento Variavel.\n");
    pop();
    pprint("FINALIZANDO <variavel>\n");
    pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 1; //erro
}

int p_mais_var(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_var>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_VIRGULA) == 0){
        obterSimbolo(entrada, linha, i, token);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            //Chama procedimento Mais_var
            p_mais_var(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <mais_var>\n");
            return 0;
        }{ // Erro nao encontrou IDENT
            char ** s1 = (char**) malloc(sizeof(char *) *3);
            for(int i = 0; i< 3; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[2], IDENT);
            strcpy(s1[1], SIMB_VIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 3);
            //obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <mais_var>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }{ // Erro nao encontrou SIMB_VIRGULA (pode retornar lambda)
        /*char ** s1 = (char**) malloc(sizeof(char *) *2);
        for(int i = 0; i< 2; i++){
            s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
        }
        strcpy(s1[1], SIMB_PVIRGULA);
        strcpy(s1[0], SIMB_PONTO);
        printf("Erro sintático: SIMB_VIRGULA faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s1, 2);
        pop();
        pprint("FINALIZANDO <mais_var>\n");*/
        return 1; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("FINALIZANDO <mais_var>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_procedimento(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <procedimento>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "PROCEDURE") == 0){
        obterSimbolo(entrada, linha, i, token);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                obterSimbolo(entrada, linha, i, token);
                //Chama procedimento Bloco
                p_bloco(entrada, linha, i, token, s, num_simb_sinc);
                if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                    obterSimbolo(entrada, linha, i, token);
                    //Chama procedimento Procedimento
                    p_procedimento(entrada, linha, i, token, s, num_simb_sinc);
                    pop();
                    pprint("FINALIZANDO <procedimento>\n");
                    return 0; // sem erros
                }{ // Erro nao encontrou SIMB_PVIRGULA
                    char ** s1 = (char**) malloc(sizeof(char *) *7);
                    for(int i = 0; i< 7; i++){
                        s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
                    }
                    strcpy(s1[6], "BEGIN");
                    strcpy(s1[5], IDENT);
                    strcpy(s1[4], "IF");
                    strcpy(s1[3], "WHILE");
                    strcpy(s1[2], "CALL");
                    strcpy(s1[1], SIMB_PVIRGULA);
                    strcpy(s1[0], SIMB_PONTO);
                    printf("Erro sintático: SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
                    p_erro(entrada, linha, i, token, s1, 7);
                    pop();
                    pprint("Erro corrigido, nao finaliza <procedimento>\n");
                    return 1; //Foi capaz de corrigir o erro
                }
            }else{ // Erro nao encontrou SIMB_PVIRGULA
                char ** s1 = (char**) malloc(sizeof(char *) *5);
                for(int i = 0; i< 5; i++){
                    s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
                }
                strcpy(s1[4], "BEGIN");
                strcpy(s1[3], "IF");
                strcpy(s1[2], "WHILE");
                strcpy(s1[1], "CALL");
                strcpy(s1[0], SIMB_PONTO);
                printf("Erro sintático: SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
                p_erro(entrada, linha, i, token, s1, 5);
                pop();
                pprint("Finaliza <procedimento>\n");
                return 1; //Foi capaz de corrigir o erro
            }
        }else{ // Erro nao encontrou IDENT
            char ** s1 = (char**) malloc(sizeof(char *) *10);
            for(int i = 0; i< 10; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[9], SIMB_PVIRGULA);
            strcpy(s1[8], "CONST");
            strcpy(s1[7], "VAR");
            strcpy(s1[6], "BEGIN");
            strcpy(s1[5], IDENT);
            strcpy(s1[4], "CALL");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 10);
            if(strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                obterSimbolo(entrada, linha, i, token);
            }
            pop();
            pprint("FINALIZANDO <procedimento>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }
    pop();
    pprint("FINALIZANDO <procedimento>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_comando(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <comando>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    //Se comando gera um atribuicao
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        obterSimbolo(entrada, linha, i, token);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_ATRIBUICAO) == 0){
            obterSimbolo(entrada, linha, i, token);
            //Chama procedimento Expressao
            p_expressao(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0;
        }else{ // Erro nao encontrou SIMB_ATRIBUICAO
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], SIMB_PVIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: SIMB_ATRIBUICAO faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "CALL") == 0){
        obterSimbolo(entrada, linha, i, token);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            return 0;
        }else{ // Erro nao encontrou IDENT
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], SIMB_PVIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: IDENT faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "BEGIN") == 0){
        obterSimbolo(entrada, linha, i, token);
        p_comando(entrada, linha, i, token, s, num_simb_sinc);
        p_mais_cmd(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "END") == 0){
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <comando>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0; //sem erros
        }else{ // Erro nao encontrou END
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], "END");
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: END faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "IF") == 0){
        obterSimbolo(entrada, linha, i, token);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "THEN") == 0){
            obterSimbolo(entrada, linha, i, token);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0; //sem erros
        }else{ // Erro nao encontrou THEN
            char ** s1 = (char**) malloc(sizeof(char *) *7);
            for(int i = 0; i< 7; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }//;, END, THEN, DO, ., ident, CALL, BEGIN, IF, WHILE
            strcpy(s1[6], "THEN");
            strcpy(s1[5], "END");
            strcpy(s1[4], "DO");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: THEN faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 7);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "WHILE") == 0){
        obterSimbolo(entrada, linha, i, token);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, "DO") == 0){
            obterSimbolo(entrada, linha, i, token);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, s, num_simb_sinc);
            pop();
            pprint("FINALIZANDO <comando>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0; //sem erros
        }else{ // Erro nao encontrou DO
            char ** s1 = (char**) malloc(sizeof(char *) *7);
            for(int i = 0; i< 7; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }//;, END, THEN, DO, ., ident, CALL, BEGIN, IF, WHILE
            strcpy(s1[6], "THEN");
            strcpy(s1[5], "END");
            strcpy(s1[4], "DO");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático: DO faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 7);
            pop();
            pprint("FINALIZANDO <comando>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }
    pop();
    pprint("FINALIZANDO <comando>\n");
    pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_cmd>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
        obterSimbolo(entrada, linha, i, token);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    pprint("Token: %s\n", (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MENOS) == 0 || strcmp((*token)->tipo, SIMB_MAIS) == 0)){
        obterSimbolo(entrada, linha, i, token);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MENOS) == 0 || strcmp((*token)->tipo, SIMB_MAIS) == 0)){
        obterSimbolo(entrada, linha, i, token);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, IDENT) == 0 || strcmp((*token)->tipo, SIMB_NUMERO) == 0)){
        obterSimbolo(entrada, linha, i, token);
        pop();
        pprint("FINALIZANDO <fator>\n");
        return 0;
    } else if(*token != NULL && strcmp((*token)->tipo, SIMB_ABRE_PARENTESE) == 0){
        obterSimbolo(entrada, linha, i, token);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, s, num_simb_sinc);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_FECHA_PARENTESE) == 0){
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <fator>\n");
            return 0; //sem erros
        }else{ // Erro nao encontrou  SIMB_FECHA_PARENTESE
            char ** s1 = (char**) malloc(sizeof(char *) *17);
            for(int i = 0; i< 17; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }//*, /, -, +, THEN, DO, =, <>, <, <=, >, >=, ;, END, )
            strcpy(s1[16], SIMB_MULTI);
            strcpy(s1[15], SIMB_DIV);
            strcpy(s1[14], SIMB_MENOS);
            strcpy(s1[13], SIMB_MAIS);
            strcpy(s1[12], "THEN");
            strcpy(s1[11], "DO");
            strcpy(s1[10], SIMB_IGUAL);
            strcpy(s1[9], SIMB_DIFERENTE);
            strcpy(s1[8], SIMB_PONTO);
            strcpy(s1[7], SIMB_MENOR);
            strcpy(s1[6], SIMB_MENOR_IGUAL);
            strcpy(s1[5], SIMB_MAIOR);
            strcpy(s1[4], SIMB_MAIOR_IGUAL);
            strcpy(s1[3], SIMB_PVIRGULA);
            strcpy(s1[2], "END");
            strcpy(s1[1], SIMB_FECHA_PARENTESE);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático:  ident | numero | ( <expressão> ) faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 17);
            pop();
            pprint("FINALIZANDO <fator>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    } else{ // Erro nao encontrou  ident | numero | ( <expressão> )
            char ** s1 = (char**) malloc(sizeof(char *) *17);
            for(int i = 0; i< 17; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }//*, /, -, +, THEN, DO, =, <>, <, <=, >, >=, ;, END, )
            strcpy(s1[16], SIMB_MULTI);
            strcpy(s1[15], SIMB_DIV);
            strcpy(s1[14], SIMB_MENOS);
            strcpy(s1[13], SIMB_MAIS);
            strcpy(s1[12], "THEN");
            strcpy(s1[11], "DO");
            strcpy(s1[10], SIMB_IGUAL);
            strcpy(s1[9], SIMB_DIFERENTE);
            strcpy(s1[8], SIMB_PONTO);
            strcpy(s1[7], SIMB_MENOR);
            strcpy(s1[6], SIMB_MENOR_IGUAL);
            strcpy(s1[5], SIMB_MAIOR);
            strcpy(s1[4], SIMB_MAIOR_IGUAL);
            strcpy(s1[3], SIMB_PVIRGULA);
            strcpy(s1[2], "END");
            strcpy(s1[1], SIMB_FECHA_PARENTESE);
            strcpy(s1[0], SIMB_PONTO);
            printf("Erro sintático:  ident | numero | ( <expressão> ) faltando na posição %d da linha %s", *i, linha);
            p_erro(entrada, linha, i, token, s1, 17);
            pop();
            pprint("FINALIZANDO <fator>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    pop();
    pprint("FINALIZANDO <fator>\n");
    return 1;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc){
    pprint("INICIANDO <mais_fatores>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && (strcmp((*token)->tipo, SIMB_MULTI) == 0 || strcmp((*token)->tipo, SIMB_DIV) == 0)){
        obterSimbolo(entrada, linha, i, token);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "ODD") == 0){
        obterSimbolo(entrada, linha, i, token);
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
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && 
        (strcmp((*token)->tipo, SIMB_IGUAL) == 0 || strcmp((*token)->tipo, SIMB_DIFERENTE) == 0 ||
         strcmp((*token)->tipo, SIMB_MENOR) == 0 || strcmp((*token)->tipo, SIMB_MENOR_IGUAL) == 0 ||
         strcmp((*token)->tipo, SIMB_MAIOR) == 0 || strcmp((*token)->tipo, SIMB_MAIOR_IGUAL) == 0)){
        obterSimbolo(entrada, linha, i, token);
        pop();
        pprint("FINALIZANDO <relacional>\n");
        return 0; //sem erros
    }else{ // Erro nao encontrou  = | <> | < | <= | > | >=
        char ** s1 = (char**) malloc(sizeof(char *) *7);
        for(int i = 0; i< 7; i++){
            s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
        }//-, +, ident, numero, (
        strcpy(s1[6], SIMB_MENOS);
        strcpy(s1[5], SIMB_MAIS);
        strcpy(s1[4], IDENT);
        strcpy(s1[3], SIMB_NUMERO);
        strcpy(s1[2], SIMB_ABRE_PARENTESE);
        strcpy(s1[1], SIMB_PVIRGULA);
        strcpy(s1[0], SIMB_PONTO);
        printf("Erro sintático:  = | <> | < | <= | > | >= faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s1, 7);
        pop();
        pprint("FINALIZANDO <relacional>\n");
        return 1; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("FINALIZANDO <relacional>\n");
    return 1; //erro 
}


