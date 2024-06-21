#ifndef PROC_SINTATICOS_H
#define PROC_SINTATICOS_H

#include "token.h"

int p_erro(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_programa(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_bloco(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_declaracao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_constante(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_mais_const(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_variavel(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_mais_var(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_procedimento(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_comando(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_mais_cmd(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_expressao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_operador_unario(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_termo(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_mais_termos(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);

int p_fator(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_mais_fatores(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_condicao(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);
int p_relacional(FILE * entrada, char * linha, int *i, Token ** token, char ** s, int num_simb_sinc);

#endif