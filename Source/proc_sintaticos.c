#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_sintaticos.h"
#include "analisador_lexico.h"
#include "simbolos.h"
#include "tabelas.h"
#include "token.h"

#define pprint for(int i = 0; PPRINT_ACTV && i < contTab; i++) printf("\t"); if(PPRINT_ACTV) printf

#define TOKEN_NULO 3

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
    /*if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
        printf("Erro Lexico: \t %s\n", (*token)->valor);
    }*/

    int flag = 0;
    while(!feof(entrada) && *token != NULL && flag == 0){
        for(int i = num_simb_sinc - 1; i>=0; i--){
            if(strcmp((*token)->tipo, s[i]) == 0){
                pprint("Encontrou simbolo de sincronizacao: %s\n", s[i]);
                flag = 1;
                return 0;
            }else{
                pprint("Falha no tratamento: %s != %s\n", (*token)->tipo, s[i]);
            }
        }
        obterSimbolo(entrada, linha, i, token);
    }
    pprint("Não encontrou simbolo de sincronizacao, chegou ao fim do programa :(\n");
    if(feof(entrada)){
        destroiToken(token);
    }
    //obterSimbolo(entrada, linha, i, token);
    return 0;
}

int p_programa(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <programa>\n");
    push();
    if(*token != NULL){
        pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

        //Detecção do menor programa possível.
        if(token != NULL && strcmp((*token)->tipo, SIMB_PONTO) == 0){
            return 0;
        }
        

        int ret = p_bloco(entrada, linha, i, token, erros);
        
        if(ret == TOKEN_NULO){ //chegou ao ultimo simbolo do arquivo
            //printf("Erro sintático: \t SIMB_PONTO faltando na posição %d da linha %s", *i, linha);
            pprint("FINALIZANDO <programa>. (token nulo)\n");
            return 1;
        }

        //Depois de um bloco deve vir um ponto final
        if(*token != NULL && strcmp((*token)->tipo, SIMB_PONTO) == 0){
            pprint("Programa terminou como esperado (leu ponto final --> leu NULL)\n");
            pop();
            pprint("FINALIZANDO <programa>.\n");
            return 0; //programa terminou como esperado
        }else{//Erro, seguidor de programa eh SIMB_PONTO
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *9);
            for(int i = 0; i< 9; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[8], SIMB_PONTO);
            strcpy(s1[7], SIMB_PVIRGULA);
            strcpy(s1[6], "CONST");
            strcpy(s1[5], "VAR");
            strcpy(s1[4], "PROCEDURE");
            strcpy(s1[3], "CALL");
            strcpy(s1[2], "BEGIN");
            strcpy(s1[1], "IF");
            strcpy(s1[0], "WHILE");
            //strcpy(s1[0], IDENT);
            if(*token != NULL && strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else{
                printf("Erro sintático: \t SIMB_PONTO faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 9);
            for(int i = 0; i< 9; i++){
                free(s1[i]);
            }
            free(s1);
            if(*token != NULL && cmpToken(*token, SIMB_PVIRGULA)){
                obterSimbolo(entrada, linha, i, token);
            }
            pop();
            pprint("FINALIZANDO <programa>\n");
            //p_programa(entrada,linha,i,token, erros);
            return 1;
        }
    }
}

int p_bloco(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <bloco>\n");
    push();
    
    if(*token != NULL){
        pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        //Verifica se o símbolo é uma declaração.
        int flag = 0;
        if(cmpToken(*token, "CONST")|| 
           cmpToken(*token, "VAR") || 
           cmpToken(*token, "PROCEDURE")){
            p_declaracao(entrada, linha, i, token, erros);
            flag = 1;
        } 
        
        if(cmpToken(*token, IDENT) || cmpToken(*token, "CALL")|| 
           cmpToken(*token, "BEGIN") || cmpToken(*token, "IF")|| 
           cmpToken(*token, "WHILE")){ //Verifica se o símbolo é um comando.
            p_comando(entrada, linha, i, token, erros);
            flag = 1;
        } 
        
        if(flag == 0){//Erro seguidor eh SIMB_PONTO
            //Adiciona o símbolo na fila de erros.
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *10);
            for(int i = 0; i< 10; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[9], SIMB_PONTO);
            strcpy(s1[8], SIMB_PVIRGULA);
            strcpy(s1[7], "CONST");
            strcpy(s1[6], "VAR");
            strcpy(s1[5], "PROCEDURE");
            strcpy(s1[4], "CALL");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], IDENT);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            } else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            } else{
                printf("Erro sintático: \t esperava CONST, VAR, PROCEDURE, BEGIN, CALL, IF, WHILE ou IDENT na posição %d da linha %s\n", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 10);
            for(int i = 0; i< 10; i++){
                free(s1[i]);
            }
            free(s1);
            if(*token != NULL && cmpToken(*token, SIMB_PVIRGULA)){
                obterSimbolo(entrada, linha, i, token);
            }
            pprint("FINALIZANDO <bloco>\n");
            if(!feof(entrada)){
                p_bloco(entrada,linha,i,token, erros);
            }
            return 1;
        }
    }

    //Obtém o próximo símbolo
    //obterSimbolo(entrada, linha, i, token);

    pop();
    pprint("FINALIZANDO <bloco>. (token nulo 2)\n");
    //pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 3;
}

int p_declaracao(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <declaracao>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    if(*token != NULL){
        if(cmpToken(*token, "CONST")){
            p_constante(entrada, linha, i, token, erros);
        }   
        if(cmpToken(*token, "VAR")){
            p_variavel(entrada, linha, i, token, erros);
        }           
        if(cmpToken(*token, "PROCEDURE")){
            p_procedimento(entrada, linha, i, token, erros);
        }      
    }

    pop();
    pprint("FINALIZANDO <declaração>\n");
    return 0;
}

int p_constante(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
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

                p_mais_const(entrada, linha, i, token, erros);

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
                else{ //VAR, PROCEDURE, ident, CALL, BEGIN, IF, WHILE
                    //Adiciona o símbolo na fila de erros.
                    (*erros)++;
                    char ** s1 = (char**) malloc(sizeof(char *) *8);
                    for(int i = 0; i< 8; i++){
                        s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
                    }
                    strcpy(s1[7], SIMB_PVIRGULA);
                    strcpy(s1[6], "VAR");
                    strcpy(s1[5], "PROCEDURE");
                    strcpy(s1[4], "CALL");
                    strcpy(s1[3], "BEGIN");
                    strcpy(s1[2], "IF");
                    strcpy(s1[1], "WHILE");
                    strcpy(s1[0], SIMB_PONTO);
                    if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                        printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                    }else{
                        printf("Erro sintático: \t SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
                    }
                    p_erro(entrada, linha, i, token, s1, 8);
                    for(int i = 0; i< 8; i++){
                        free(s1[i]);
                    }
                    free(s1);
                    if(cmpToken(*token, SIMB_PVIRGULA)){
                        obterSimbolo(entrada, linha, i, token);
                    }
                    pop();
                    pprint("FINALIZANDO <constante>\n");
                    return 0; //Foi capaz de corrigir o erro
                }
            }
            //Erro ao encontrar um NUMERO.
            else{ //VAR, PROCEDURE, ident, CALL, BEGIN, IF, WHILE
                //Adiciona o símbolo na fila de erros.
                (*erros)++;
                char ** s1 = (char**) malloc(sizeof(char *) *8);
                for(int i = 0; i< 8; i++){
                    s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
                }
                strcpy(s1[7], SIMB_PVIRGULA);
                strcpy(s1[6], "VAR");
                strcpy(s1[5], "PROCEDURE");
                strcpy(s1[4], "CALL");
                strcpy(s1[3], "BEGIN");
                strcpy(s1[2], "IF");
                strcpy(s1[1], "WHILE");
                strcpy(s1[0], SIMB_PONTO);
                if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                    printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                }else{
                    printf("Erro sintático: \t SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                }
                
                p_erro(entrada, linha, i, token, s1, 8);
                for(int i = 0; i< 8; i++){
                    free(s1[i]);
                }
                free(s1);
                if(cmpToken(*token, SIMB_PVIRGULA)){
                    obterSimbolo(entrada, linha, i, token);
                }
                pop();
                pprint("FINALIZANDO <constante>\n");
                return 0; //Foi capaz de corrigir o erro
            }   
        }
        //Erro ao encontrar SIMB_IGUAL.
        else{ //VAR, PROCEDURE, ident, CALL, BEGIN, IF, WHILE
            //Adiciona o símbolo na fila de erros.
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *8);
            for(int i = 0; i< 8; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[7], SIMB_PVIRGULA);
            strcpy(s1[6], "VAR");
            strcpy(s1[5], "PROCEDURE");
            strcpy(s1[4], "CALL");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
            }
            
            p_erro(entrada, linha, i, token, s1, 8);
            for(int i = 0; i< 8; i++){
                free(s1[i]);
            }
            free(s1);
            if(cmpToken(*token, SIMB_PVIRGULA)){
                obterSimbolo(entrada, linha, i, token);
            }
            pop();
            pprint("FINALIZANDO <constante>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }
    //Erro ao encontrar um IDENT.
    else{ //VAR, PROCEDURE, ident, CALL, BEGIN, IF, WHILE
        //Adiciona o símbolo na fila de erros.
        (*erros)++;
        char ** s1 = (char**) malloc(sizeof(char *) *8);
        for(int i = 0; i< 8; i++){
            s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
        }
        strcpy(s1[7], SIMB_PVIRGULA);
        strcpy(s1[6], "VAR");
        strcpy(s1[5], "PROCEDURE");
        strcpy(s1[4], "CALL");
        strcpy(s1[3], "BEGIN");
        strcpy(s1[2], "IF");
        strcpy(s1[1], "WHILE");
        strcpy(s1[0], SIMB_PONTO);
        if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
            printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
        }else{
            printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
        }
        p_erro(entrada, linha, i, token, s1, 8);
        for(int i = 0; i< 8; i++){
            free(s1[i]);
        }
        free(s1);
        if(cmpToken(*token, SIMB_PVIRGULA)){
            obterSimbolo(entrada, linha, i, token);
        }
        pop();
        pprint("FINALIZANDO <constante>\n");
        return 0; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("ERRO: FINALIZANDO <constante>\n");
    return 1;
}

int p_mais_const(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
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
                    p_mais_const(entrada, linha, i, token, erros);

                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0;
                }
                //Erro ao encontrar um SIMB_NUMERO.
                else{
                    //Adiciona o símbolo na fila de erros.
                    (*erros)++;
                    char ** s = (char**) realloc(s,sizeof(char *)*2);
                    s[0] = malloc(sizeof(char) * TAM_SIMBOLO);
                    s[1] = malloc(sizeof(char) * TAM_SIMBOLO);
                    strcpy(s[0], SIMB_PVIRGULA);
                    strcpy(s[1], SIMB_NUMERO);

                    if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                        printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                    }else{
                        printf("Erro sintático: \t SIMB_NUMERO faltando na posição %d da linha %s", *i, linha);
                    }
                    p_erro(entrada, linha, i, token, s, 2);
                    //obterSimbolo(entrada, linha, i, token);

                    pop();
                    pprint("FINALIZANDO <mais_const>\n");
                    return 0; //Foi capaz de corrigir o erro
                }
            }
            //Erro ao encontrar um SIMB_IGUAL.
            else{
                //Adiciona o símbolo na fila de erros.
                (*erros)++;
                char ** s = (char**) realloc(s,sizeof(char *));
                s[0] = malloc(sizeof(char)*TAM_SIMBOLO);
                //s[num_simb_sinc + 1] = malloc(sizeof(char)*TAM_SIMBOLO);
                strcpy(s[0], SIMB_PVIRGULA);
                //strcpy(s[num_simb_sinc + 1], SIMB_IGUAL);

                
                if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                    printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                }else{
                    printf("Erro sintático: \t SIMB_IGUAL faltando na posição %d da linha %s", *i, linha);
                }
                p_erro(entrada, linha, i, token, s, 1);
                obterSimbolo(entrada, linha, i, token);

                pop();
                pprint("FINALIZANDO <mais_const>\n");
                return 0; //Foi capaz de corrigir o erro
            }
        }
        //Erro ao encontrar um IDENT.
        else{ //VAR, PROCEDURE, ident, CALL, BEGIN, IF, WHILE
            //Adiciona o símbolo na fila de erros.
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *));
            s1[0] = malloc(sizeof(char)*TAM_SIMBOLO);
            strcpy(s1[0], SIMB_PVIRGULA);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 1);
            free(s1[0]);
            free(s1);
            pop();
            pprint("FINALIZANDO <mais_const>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }

    pop();
    pprint("FINALIZANDO <mais_const>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_variavel(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <variavel>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    obterSimbolo(entrada, linha, i, token);
    pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("IDENT encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        p_mais_var(entrada, linha, i, token, erros);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("SIMB_PVIRGULA encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            pop();
            pprint("FINALIZANDO <variavel>\n");
            pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            return 0;
        }//Erro ao encontrar um SIMB_PVIRGULA.
        else{
            //Adiciona o símbolo na fila de erros.
            (*erros)++;
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
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 7);
            if(cmpToken(*token, SIMB_PVIRGULA)){
                obterSimbolo(entrada, linha, i, token);
            }
            pop();
            for(int i = 0; i< 7; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <variavel>\n");
            return 0; //Foi capaz de corrigir o erro
        }
    }//Erro ao encontrar um IDENT.
    else{
        (*erros)++;
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
        if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
            printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
        }else{
            printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
        }
        p_erro(entrada, linha, i, token, s1, 7);
        if(cmpToken(*token, SIMB_PVIRGULA)){
            obterSimbolo(entrada, linha, i, token);
        }
        pop();
        for(int i = 0; i< 7; i++){
            free(s1[i]);
        }
        free(s1);
        pprint("FINALIZANDO <variavel>\n");
        pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        return 1; //Foi capaz de corrigir o erro
    }
    pprint("ERRO: procedimento Variavel.\n");
    pop();
    pprint("FINALIZANDO <variavel>\n");
    pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    return 1; //erro
}

int p_mais_var(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <mais_var>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_VIRGULA) == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("SIMB_VIRGULA encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("IDENT encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            //Chama procedimento Mais_var
            p_mais_var(entrada, linha, i, token, erros);
            pop();
            pprint("FINALIZANDO <mais_var>\n");
            return 0;
        }{ // Erro nao encontrou IDENT
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *3);
            for(int i = 0; i< 3; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[2], IDENT);
            strcpy(s1[1], SIMB_VIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 3);
            //obterSimbolo(entrada, linha, i, token);
            pop();for(int i = 0; i< 3; i++){
                free(s1[i]);
            }
            free(s1);
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
        printf("Erro sintático: \t SIMB_VIRGULA faltando na posição %d da linha %s", *i, linha);
        p_erro(entrada, linha, i, token, s1, 2);
        pop();
        pprint("FINALIZANDO <mais_var>\n");*/
        return 1; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("FINALIZANDO <mais_var>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_procedimento(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <procedimento>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, "PROCEDURE") == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("PROCEDURE encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                obterSimbolo(entrada, linha, i, token);
                pprint("SIMB_PVIRGULA encontrado!\n");
                pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
                //Chama procedimento Bloco
                p_bloco(entrada, linha, i, token, erros);
                if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                    obterSimbolo(entrada, linha, i, token);
                    pprint("SIMB_PVIRGULA encontrado!\n");
                    pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
                    //Chama procedimento Procedimento
                    p_procedimento(entrada, linha, i, token, erros);
                    pop();
                    pprint("FINALIZANDO <procedimento>\n");
                    return 0; // sem erros
                }{ // Erro nao encontrou SIMB_PVIRGULA
                    (*erros)++;
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
                    if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                        printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                    }else{
                        printf("Erro sintático: \t SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
                    }
                    p_erro(entrada, linha, i, token, s1, 7);
                    pop();
                    for(int i = 0; i< 7; i++){
                        free(s1[i]);
                    }
                    free(s1);
                    pprint("Erro corrigido, nao finaliza <procedimento>\n");
                    return 1; //Foi capaz de corrigir o erro
                }
            }else{ // Erro nao encontrou SIMB_PVIRGULA
                (*erros)++;
                char ** s1 = (char**) malloc(sizeof(char *) *5);
                for(int i = 0; i< 5; i++){
                    s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
                }
                strcpy(s1[4], "BEGIN");
                strcpy(s1[3], "IF");
                strcpy(s1[2], "WHILE");
                strcpy(s1[1], "CALL");
                strcpy(s1[0], SIMB_PONTO);
                if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                    printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
                }else{
                    printf("Erro sintático: \t SIMB_PVIRGULA faltando na posição %d da linha %s", *i, linha);
                }
                p_erro(entrada, linha, i, token, s1, 5);
                pop();
                for(int i = 0; i< 5; i++){
                    free(s1[i]);
                }
                free(s1);
                pprint("Finaliza <procedimento>\n");
                return 1; //Foi capaz de corrigir o erro
            }
        }else{ // Erro nao encontrou IDENT
            (*erros)++;
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
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 10);
            if(strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
                obterSimbolo(entrada, linha, i, token);
            }
            pop();
            for(int i = 0; i< 10; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <procedimento>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }
    pop();
    pprint("FINALIZANDO <procedimento>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_comando(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <comando>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    //Se comando gera um atribuicao
    if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("IDENT encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_ATRIBUICAO) == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("SIMB_ATRIBUICAO encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            //Chama procedimento Expressao
            p_expressao(entrada, linha, i, token, erros);
            pop();
            pprint("FINALIZANDO <comando> 1\n");
            if(*token != NULL){
                pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            }
            return 0;
        }else{ // Erro nao encontrou SIMB_ATRIBUICAO
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], SIMB_PVIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t SIMB_ATRIBUICAO faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            for(int i = 0; i< 2; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <comando> 2\n");
            if(*token != NULL){
                obterSimbolo(entrada, linha, i, token);
                //Chama procedimento Comando
                if(*token != NULL){
                    pprint("ttoken: (%s,%s)\n", (*token)->valor, (*token)->tipo);
                    p_comando(entrada, linha, i, token, erros);
                }   
            }
            return 1; //Foi capaz de corrigir o erro
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "CALL") == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("CALL encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        if(*token != NULL && strcmp((*token)->tipo, IDENT) == 0){
            obterSimbolo(entrada, linha, i, token);
            return 0;
        }else{ // Erro nao encontrou IDENT
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], SIMB_PVIRGULA);
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t IDENT faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            for(int i = 0; i< 2; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <comando> 3\n");
            return 1; //Foi capaz de corrigir o erro
        }
    } else if(*token != NULL && strcmp((*token)->tipo, "BEGIN") == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("BEGIN encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        p_comando(entrada, linha, i, token, erros);
        p_mais_cmd(entrada, linha, i, token, erros);
        if(*token != NULL && strcmp((*token)->tipo, "END") == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("END encontrado!\n");
            pop();
            pprint("FINALIZANDO <comando> 4\n");
            return 0; //sem erros
        }else{ // Erro nao encontrou END
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *2);
            for(int i = 0; i< 2; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }
            strcpy(s1[1], "END");
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){ 
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t END faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 2);
            pop();
            for(int i = 0; i< 2; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <comando> 5\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "IF") == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("IF encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, erros);
        if(*token != NULL && strcmp((*token)->tipo, "THEN") == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("THEN encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, erros);
            pop();
            pprint("FINALIZANDO <comando> 6\n");
            if(*token != NULL){
                pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            }
            return 0; //sem erros
        }else{ // Erro nao encontrou THEN
            (*erros)++;
            char ** s1 = (char**) malloc(sizeof(char *) *8);
            for(int i = 0; i< 8; i++){
                s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
            }//;, END, THEN, DO, ., ident, CALL, BEGIN, IF, WHILE
            strcpy(s1[7], "THEN");
            strcpy(s1[6], IDENT);
            strcpy(s1[5], "END");
            strcpy(s1[4], "DO");
            strcpy(s1[3], "BEGIN");
            strcpy(s1[2], "IF");
            strcpy(s1[1], "WHILE");
            strcpy(s1[0], SIMB_PONTO);
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t THEN faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 8);
            pop();
            for(int i = 0; i< 8; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <comando> 7\n");
            if(*token != NULL && (cmpToken(*token, IDENT) || cmpToken(*token, "CALL") ||
                 cmpToken(*token, "BEGIN") || cmpToken(*token, "IF") || cmpToken(*token, "WHILE"))){
                p_comando(entrada, linha, i, token, erros);
            }
            return 1; //Foi capaz de corrigir o erro
        }
    }else if(*token != NULL && strcmp((*token)->tipo, "WHILE") == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("WHILE encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        //Chama procedimento Condicao
        p_condicao(entrada, linha, i, token, erros);
        if(*token != NULL && strcmp((*token)->tipo, "DO") == 0){
            obterSimbolo(entrada, linha, i, token);
            pprint("DO encontrado!\n");
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            //Chama procedimento Comando
            p_comando(entrada, linha, i, token, erros);
            pop();
            pprint("FINALIZANDO <comando> 8\n");
            if(*token != NULL){
                pprint("Simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            }
            return 0; //sem erros
        }else{ // Erro nao encontrou DO
            (*erros)++;
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
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t DO faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 7);
            pop();
            for(int i = 0; i< 7; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <comando> 9\n");
            return 1; //Foi capaz de corrigir o erro
        }
    }
    pop();
    pprint("FINALIZANDO <comando> 10\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_mais_cmd(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <mais_cmd>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && strcmp((*token)->tipo, SIMB_PVIRGULA) == 0){
        obterSimbolo(entrada, linha, i, token);
        pprint("SIMB_PVIRGULA encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        //Chama procedimento Comando
        p_comando(entrada, linha, i, token, erros);
        //Chama procedimento Mais_cmd
        p_mais_cmd(entrada, linha, i, token, erros);
        pop();
        pprint("FINALIZANDO <mais_cmd>\n");
        return 0;
    }
    pop();
    pprint("FINALIZANDO <mais_cmd>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_expressao(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <expressao>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    //Chama procedimento Operador unario
    p_operador_unario(entrada, linha, i, token, erros);
    //Chama procedimento Termo
    p_termo(entrada, linha, i, token, erros);
    //Chama procedimento Mais Termos
    p_mais_termos(entrada, linha, i, token, erros);
    pop();
    pprint("FINALIZANDO <expressao>\n");
    return 0;
}

int p_operador_unario(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <operador_unario>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    
    if(*token != NULL){
        if(cmpToken(*token, SIMB_MENOS) || cmpToken(*token, SIMB_MAIS)){
            pprint("%s encontrado!\n", (*token)->tipo);
            obterSimbolo(entrada, linha, i, token);
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
            pop();
            pprint("FINALIZANDO <operador_unario>\n");
            return 0; //sem erros
        }
    }

    pop();
    pprint("FINALIZANDO <operador_unario>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_termo(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <termo>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);

    p_fator(entrada, linha, i, token, erros);
    p_mais_fatores(entrada, linha, i, token, erros);

    pop();
    pprint("FINALIZANDO <termo>\n");
    return 0;
}

int p_mais_termos(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <mais_termos>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    
    if(*token != NULL){
        if(cmpToken(*token, SIMB_MENOS) || cmpToken(*token, SIMB_MAIS)){
            pprint("%s encontrado!\n", (*token)->tipo);
            obterSimbolo(entrada, linha, i, token);
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

            p_termo(entrada, linha, i, token, erros);
            p_mais_termos(entrada, linha, i, token, erros);
            pop();
            pprint("FINALIZANDO <mais_termos>\n");
            return 0;
        }
    }

    pop();
    pprint("FINALIZANDO <mais_termos>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_fator(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
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
        p_expressao(entrada, linha, i, token, erros);
        if(*token != NULL && strcmp((*token)->tipo, SIMB_FECHA_PARENTESE) == 0){
            obterSimbolo(entrada, linha, i, token);
            pop();
            pprint("FINALIZANDO <fator>\n");
            return 0; //sem erros
        }else{ // Erro nao encontrou  SIMB_FECHA_PARENTESE
            (*erros)++;
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
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t  ident | numero | ( <expressão> ) faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 17);
            pop();
            for(int i = 0; i< 17; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <fator>\n");
            if(*token != NULL && (strcmp((*token)->tipo, SIMB_FECHA_PARENTESE) == 0 || strcmp((*token)->tipo, SIMB_PVIRGULA) == 0)){
                obterSimbolo(entrada, linha, i, token);
            }
            return 1; //Foi capaz de corrigir o erro
        }
    } else{ // Erro nao encontrou  ident | numero | ( <expressão> )
            (*erros)++;
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
            if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
                printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
            }else if(strcmp((*token)->tipo, ERRO_EOF) == 0){
                printf("Erro: \t Fim de arquivo inesperado na linha %s", linha);
            }else{
                printf("Erro sintático: \t  ident | numero | ( <expressão> ) faltando na posição %d da linha %s", *i, linha);
            }
            p_erro(entrada, linha, i, token, s1, 17);
            pop();
            for(int i = 0; i< 17; i++){
                free(s1[i]);
            }
            free(s1);
            pprint("FINALIZANDO <fator>\n");
            return 1; //Foi capaz de corrigir o erro
        }
    pop();
    pprint("FINALIZANDO <fator>\n");
    return 1;
}

int p_mais_fatores(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <mais_fatores>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    
    if(*token != NULL){
        if(cmpToken(*token, SIMB_MULTI) || cmpToken(*token, SIMB_DIV)){
            pprint("%s encontrado!\n", (*token)->tipo);
            obterSimbolo(entrada, linha, i, token);
            pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);

            p_fator(entrada, linha, i, token, erros);
            p_mais_fatores(entrada, linha, i, token, erros);

            pop();
            pprint("FINALIZANDO <mais_fatores>\n");
            return 0; // sem erros
        }
    }
    
    pop();
    pprint("FINALIZANDO <mais_fatores>\n");
    return 0; //coloquei que retorna 0 (OK) pois pode gerar lambda (???)
}

int p_condicao(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
    pprint("INICIANDO <condicao>\n");
    push();
    pprint("Procedimento inicia com token: (%s,%s)\n", (*token)->valor, (*token)->tipo);
    if(*token != NULL && cmpToken(*token, "ODD")){
        obterSimbolo(entrada, linha, i, token);
        pprint("ODD encontrado!\n");
        pprint("Proximo simbolo: (%s,%s)\n", (*token)->valor, (*token)->tipo);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, erros);
        pop();
        pprint("FINALIZANDO <condicao>: ODD\n");
        return 0; //sem erros
    }
    else{
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, erros);
        //Chama procedimento Relacional
        p_relacional(entrada, linha, i, token, erros);
        //Chama procedimento Expressao
        p_expressao(entrada, linha, i, token, erros);
        pop();
        pprint("FINALIZANDO <condicao>\n");
        return 0; //sem erros
    }
    pop();
    pprint("FINALIZANDO <condicao>\n");
    return 1; //erro
}

int p_relacional(FILE * entrada, char * linha, int *i, Token ** token, int * erros){
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
        (*erros)++;
        char ** s1 = (char**) malloc(sizeof(char *) *9);
        for(int i = 0; i< 9; i++){
            s1[i] = malloc(sizeof(char)*TAM_SIMBOLO);
        }//-, +, ident, numero, (
        strcpy(s1[8], SIMB_MENOS);
        strcpy(s1[7], SIMB_MAIS);
        strcpy(s1[6], IDENT);
        strcpy(s1[5], SIMB_NUMERO);
        strcpy(s1[4], SIMB_ABRE_PARENTESE);
        strcpy(s1[3], SIMB_PVIRGULA);
        strcpy(s1[2], "THEN");
        strcpy(s1[1], "DO");
        strcpy(s1[0], SIMB_PONTO);
        if(strcmp((*token)->tipo, ERRO_LEXICO) == 0){
            printf("Erro Lexico: \t %s na posição %d da linha %s", (*token)->valor, *i, linha);
        }else{
            printf("Erro sintático: \t  = | <> | < | <= | > | >= faltando na posição %d da linha %s", *i, linha);
        }
        p_erro(entrada, linha, i, token, s1, 9);
        pop();
        for(int i = 0; i< 9; i++){
            free(s1[i]);
        }
        free(s1);
        pprint("FINALIZANDO <relacional>\n");
        return 1; //Foi capaz de corrigir o erro
    }
    pop();
    pprint("FINALIZANDO <relacional>\n");
    return 1; //erro 
}


