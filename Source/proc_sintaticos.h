#ifndef PROC_SINTATICOS_H
#define PROC_SINTATICOS_H

int p_programa(FILE * entrada, char * linha, int i, char ** s);
int p_bloco(FILE * entrada, char * linha, int i, char ** s);
int p_declaracao(FILE * entrada, char * linha, int i, char ** s);
int p_constante(FILE * entrada, char * linha, int i, char ** s);
int p_mais_const(FILE * entrada, char * linha, int i, char ** s);
int p_variavel(FILE * entrada, char * linha, int i, char ** s);
int p_mais_var(FILE * entrada, char * linha, int i, char ** s);
int p_procedimento(FILE * entrada, char * linha, int i, char ** s);
int p_comando(FILE * entrada, char * linha, int i, char ** s);
int p_mais_cmd(FILE * entrada, char * linha, int i, char ** s);
int p_expressao(FILE * entrada, char * linha, int i, char ** s);
int p_operador_unario(FILE * entrada, char * linha, int i, char ** s);
int p_termo(FILE * entrada, char * linha, int i, char ** s);
int p_mais_termos(FILE * entrada, char * linha, int i, char ** s);
int p_fator(FILE * entrada, char * linha, int i, char ** s);
int p_mais_fatores(FILE * entrada, char * linha, int i, char ** s);
int p_condicao(FILE * entrada, char * linha, int i, char ** s);
int p_relacional(FILE * entrada, char * linha, int i, char ** s);

#endif