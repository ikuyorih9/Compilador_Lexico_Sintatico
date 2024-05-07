#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_saida.h"
#include "tabelas.h"
#include "simbolos.h"

#define STREAM_SAIDA saida


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
@param saida arquivo de saída.
@return (0) se a função não retrocede, (1) se a função retrocede e (-1) se não há função de saída associada ao estado.
*/
int procuraFuncaoSaida(char * estado, char * palavra, FILE * saida){
    for(int i = 0; i < NUM_ESTADOS_SAIDA; i++){
        int encontrouEstado = !strcmp(estado, tabelaFuncoes[i].estado);
        if(encontrouEstado){
            tabelaFuncoes[i].funcaoSaida(palavra,saida);
            return tabelaFuncoes[i].retrocede;
        }
    }
    return -1;
}

void funcaoSaida0(char * palavra, FILE * saida){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    //Verifica se é palavra reservada.
    int palavraReservada = verificaSePalavraReservada(palavra);
    if(palavraReservada)
        fprintf(STREAM_SAIDA, "%s, %s\n",palavra,palavra);
    else
        fprintf(STREAM_SAIDA, "%s, %s\n",palavra,IDENT);

    return;
}

void funcaoSaida1(char * palavra, FILE * saida){
    return;
}

void funcaoSaida2(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_VIRGULA);
    return;
}

void funcaoSaida3(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_PVIRGULA);
    return;
}

void funcaoSaida4(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_ABRE_PARENTESE);
    return;
}

void funcaoSaida5(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_FECHA_PARENTESE);
    return;
}

void funcaoSaida6(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MAIS);
    return;
}

void funcaoSaida7(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MENOS);
    return;
}

void funcaoSaida8(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MULTI);
    return;
}

void funcaoSaida9(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_PONTO);
    return;
}

void funcaoSaida10(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_DIV);
    return;
}

void funcaoSaida11(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_IGUAL);
    return;
}

void funcaoSaida12(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_ATRIBUICAO);
    return;
}

void funcaoSaida13(char * palavra, FILE * saida){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_NUMERO);
    return;
}

void funcaoSaida14(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MENOR_IGUAL);
    return;
}

void funcaoSaida15(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_DIFERENTE);
    return;
}

void funcaoSaida16(char * palavra, FILE * saida){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MENOR);
    return;
}

void funcaoSaida17(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MENOR_IGUAL);
    return;
}

void funcaoSaida18(char * palavra, FILE * saida){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, SIMB_MAIOR);
    return;
}

void funcaoSaida19(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, COMENTARIO);
    return;
}

void funcaoErro1(char * palavra, FILE * saida){
    //Ignora a ultima letra que é um caractere invalido.
    int tam = strlen(palavra);
    palavra[tam-1] = '\0';

    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, ERRO_LEXICO);
    return;
}

void funcaoErro2(char * palavra, FILE * saida){
    fprintf(STREAM_SAIDA, "%s, %s\n",palavra, ERRO_LEXICO);
    return;
}