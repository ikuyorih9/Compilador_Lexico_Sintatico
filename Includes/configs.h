#ifndef CONFIGS_H
#define CONFIGS_H

#define MAX_PALAVRA 256
//Tamanho máximo de uma linha lida do arquivo de entrada.
#define MAX_LINHA 1024

//Tamanho máximo de um símbolo lido.
#define TAM_SIMBOLO 32

//Quantidade de palavras reservadas definidas.
#define NUM_PALAVRAS_RESERVADAS 11

//Quantidade de estados de saída.
#define NUM_ESTADOS_SAIDA 22

//Estado inicial do AFD.
#define ESTADO_INICIAL "Q0"

//Diretórios dos arquivos.
#define TRANSICAO_PATH "Tabelas/transicoes.txt"
#define PALAVRAS_RESERVADAS_PATH "Tabelas/palavras_reservadas.txt"

//Imprime a mensagems somente se DEBUG_PRINT = 1.
#define dprint if(DEBUG_PRINT) printf
//Controle das mensagens de debug.
#define DEBUG_PRINT 0
//Controle das mensagens do analisador sintático.
#define PPRINT_ACTV 1
//TIPOS DE DADOS

/*
Tipo de dado que representa um token
@param char*:valor representa o valor do token. 
@param char*:tipo representa o tipo do token. 
*/
typedef struct{
    char * valor;
    char * tipo;
} Token;
/*
Tipo de dados que representa uma transição na tabela de transições.
@param char[4]:estadoCorrente  estado atual do AFD.
@param char:entrada carácter lido da cadeia.
@param char[4]:estadoProx próximo estado do AFD.
*/
typedef struct{
    char estadoCorrente[4];
    char entrada;
    char estadoProx[4];
} Transicao;

/*
Tipo de dado que associa um estado, uma função e um valor de retroceder.
@param char[4]:estado representa o estado do AFD.
@param Token*:*funcaoSaida(char*) representa o ponteiro para a função a ser executada. 
@param int:retrocede indica se a função retrocede ou não. 
*/
typedef struct{
    char estado[4];
    Token * (*funcaoSaida)(char*);
    int retrocede;
} Funcao;

#endif