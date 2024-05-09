#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "simbolos.h"

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
    {{'E','2'}, funcaoErro2, 0}
};

/*
Procura a função a se executar, dependendo do estado inserido.
@param estado estado que executa uma função de saída.
@param palavra palavra lida pelo autômato.
@param saida ponteiro para uma string de saída.
@return (0) se a função não retrocede, (1) se a função retrocede e (-1) se não há função de saída associada ao estado.
*/
int procuraFuncaoSaida(char * estado, char * token, char ** saida){
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

char * funcaoSaida0(char * palavra){
    //Ignora o último caractere lido: retrocede.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    char * saida = (char*) malloc(MAX_LINHA);

    //Verifica se é palavra reservada.
    int palavraReservada = verificaSePalavraReservada(palavra);
    //Se é reservada, identifica com o próprio nome.
    if(palavraReservada){
        strcpy(saida, palavra);
        strcat(saida,", ");
        strcat(saida,palavra);
    }
    //Se ela não é reservada, identifica como identificador.
    else{
        strcpy(saida, palavra);
        strcat(saida,", ");
        strcat(saida,IDENT);
    }
    return saida;
}

char * funcaoSaida1(char * palavra){
    return NULL;
}

char * funcaoSaida2(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_VIRGULA);
    return saida;
}

char * funcaoSaida3(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_PVIRGULA);
    return saida;
}

char * funcaoSaida4(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_ABRE_PARENTESE);
    return saida;
}

char * funcaoSaida5(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_FECHA_PARENTESE);
    return saida;
}

char * funcaoSaida6(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MAIS);
    return saida;
}

char * funcaoSaida7(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MENOS);
    return saida;
}

char * funcaoSaida8(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MULTI);
    return saida;
}

char * funcaoSaida9(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_PONTO);
    return saida;
}

char * funcaoSaida10(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_DIV);
    return saida;
}

char * funcaoSaida11(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_IGUAL);
    return saida;
}

char * funcaoSaida12(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_ATRIBUICAO);
    return saida;
}

char * funcaoSaida13(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_NUMERO);
    return saida;
}

char * funcaoSaida14(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MENOR_IGUAL);
    return saida;
}

char * funcaoSaida15(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_DIFERENTE);
    return saida;
}

char * funcaoSaida16(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MENOR);
    return saida;
}

char * funcaoSaida17(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MAIOR_IGUAL);
    return saida;
}

char * funcaoSaida18(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,SIMB_MAIOR);
    return saida;
}

char * funcaoSaida19(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,COMENTARIO);
    return saida;
}

char * funcaoErro1(char * palavra){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,ERRO_LEXICO);
    return saida;
}

char * funcaoErro2(char * palavra){
    char * saida = (char*) malloc(MAX_LINHA);
    strcpy(saida, palavra);
    strcat(saida,", ");
    strcat(saida,ERRO_LEXICO);
    return saida;
}