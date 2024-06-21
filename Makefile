CC = gcc
SRC := Source
EXE := analisador_lexico
ENTRADAS := Entradas
INCLUDES := Includes
SAIDA_LOG := Logs/log.txt

C_FILES := \
	$(SRC)/tabelas.c \
	$(SRC)/funcoes_saida.c \
	$(SRC)/analisador_lexico.c \
	$(SRC)/proc_sintaticos.c \
	$(SRC)/token.c \
	$(SRC)/main.c

all: clean $(EXE) run

$(EXE): $(SRC)/main.c
	$(CC) -I $(INCLUDES) $(C_FILES) -o $(EXE).out

run:
	./$(EXE).out $(IN) | tee $(SAIDA_LOG)

clean:
	rm -f Logs/*.txt
	rm -f *.out


.PHONY:  run clean