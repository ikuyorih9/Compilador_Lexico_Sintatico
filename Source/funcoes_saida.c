#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "simbolos.h"
#include "token.h"

/*
Tabela de funções de saída para cada estado de saída.
*/
const Funcao tabelaFuncoes[] = {
    {{'S','0'}, funcaoSaida0, 1},
    {{'S','1'}, funcaoSaida1, 0},
    {{'S','2'}, funcaoSaida2, 0},
    {{'S','3'}, funcaoSaida3, 0},
    {{'S','4'}, funcaoSaida4, 0},
    {{'S','5'}, funcaoSaida5, 0},
    {{'S','6'}, funcaoSaida6, 0},
    {{'S','7'}, funcaoSaida7, 0},
    {{'S','8'}, funcaoSaida8, 0},
    {{'S','9'}, funcaoSaida9, 0},
    {{'S','1','0'}, funcaoSaida10, 0},
    {{'S','1','1'}, funcaoSaida11, 0},
    {{'S','1', '2'}, funcaoSaida12, 0},
    {{'S','1', '3'}, funcaoSaida13, 1},
    {{'S','1', '4'}, funcaoSaida14, 0},
    {{'S','1', '5'}, funcaoSaida15, 0},
    {{'S','1', '6'}, funcaoSaida16, 1},
    {{'S','1', '7'}, funcaoSaida17, 0},
    {{'S','1', '8'}, funcaoSaida18, 1},
    {{'S','1', '9'}, funcaoSaida19, 0},
    {{'E','1'}, funcaoErro1, 1},
    {{'E','2'}, funcaoErro2, 0},
    {{'E','O','F'}, funcaoSaidaEOF, 0}
};

/*
Procura a função a se executar, dependendo do estado inserido.
@param estado estado que executa uma função de saída.
@param palavra palavra lida pelo autômato.
@param saida ponteiro para uma string de saída.
@return (0) se a função não retrocede, (1) se a função retrocede e (-1) se não há função de saída associada ao estado.
*/
int procuraFuncaoSaida(char * estado, char * token, Token ** saida){
    //Percorre todos os estados de saída da tabela tabelaFuncoes.
    for(int i = 0; i < NUM_ESTADOS_SAIDA; i++){
        //Se o estado de saída lido da tabela for igual ao estado de saída recebido pela função.
        int encontrouEstado = !strcmp(estado, tabelaFuncoes[i].estado);
        if(encontrouEstado){
            //Recupera a identificação do token 'palavra'.
            *saida = tabelaFuncoes[i].funcaoSaida(token);

            //Retorna se retrocede ou não, conforme a tabelaFuncoes.
            return tabelaFuncoes[i].retrocede;
        }
    }
    return -1;
}

Token * funcaoSaidaEOF(char * palavra){    
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);

    //Verifica se é palavra reservada.
    int palavraReservada = verificaSePalavraReservada(palavra);

    strcpy(token->tipo, ERRO_EOF);

    return token;
}

Token * funcaoSaida0(char * palavra){
    //Ignora o último caractere lido: retrocede.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';
    
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);

    //Verifica se é palavra reservada.
    int palavraReservada = verificaSePalavraReservada(palavra);
    //Se é reservada, identifica com o próprio nome.
    if(palavraReservada)
        strcpy(token->tipo, palavra);
    //Se ela não é reservada, identifica como identificador.
    else
        strcpy(token->tipo,IDENT);

    return token;
}

Token * funcaoSaida1(char * palavra){
    return NULL;
}

Token * funcaoSaida2(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_VIRGULA);
    return token;
}

Token * funcaoSaida3(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_PVIRGULA);
    return token;
}

Token * funcaoSaida4(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_ABRE_PARENTESE);
    return token;
}

Token * funcaoSaida5(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_FECHA_PARENTESE);
    return token;
}

Token * funcaoSaida6(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MAIS);
    return token;
}

Token * funcaoSaida7(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MAIS);
    return token;
}

Token * funcaoSaida8(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MULTI);
    return token;
}

Token * funcaoSaida9(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_PONTO);
    return token;
}

Token * funcaoSaida10(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_DIV);
    return token;
}

Token * funcaoSaida11(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_IGUAL);
    return token;
}

Token * funcaoSaida12(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_ATRIBUICAO);
    return token;
}

Token * funcaoSaida13(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_NUMERO);
    return token;
}

Token * funcaoSaida14(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MENOR_IGUAL);
    return token;
}

Token * funcaoSaida15(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_DIFERENTE);
    return token;
}

Token * funcaoSaida16(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MENOR);
    return token;
}

Token * funcaoSaida17(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MAIOR_IGUAL);
    return token;
}

Token * funcaoSaida18(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, SIMB_MAIOR);
    return token;
}

Token * funcaoSaida19(char * palavra){
    return NULL;
}

Token * funcaoErro1(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, ERRO_LEXICO);
    return token;
}

Token * funcaoErro2(char * palavra){
    Token * token = inicializaToken();
    strcpy(token->valor, palavra);
    strcpy(token->tipo, ERRO_LEXICO);
    return token;
}