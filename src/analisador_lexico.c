#include "analisador_lexico.h"
#include "tabelas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Verifica se a palavra lida é uma palavra reservada.
@param palavra palavra lida na linha do código.
@return (0) não é palavra reservada; (1) é palavra reservada.
*/
// int verificaPalavraReservada(char * palavra){
//     for(int i = 0; i < NUM_PALAVRAS_RESERVADAS; i++){
//         char aux[10];
//         int palavraReservada = !strcmp(palavra, tabelaPalavrasReservadas[i]);
//         if(palavraReservada)
//             return 1;
//     }
//     return 0;
// }
