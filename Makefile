CC = gcc
SRC := Source
EXE := analisador_lexico
SAIDA_LOG := Logs/log.txt

C_FILES := \
	$(SRC)/tabelas.c \
	$(SRC)/funcoes_saida.c \
	$(SRC)/analisador_lexico.c \
	$(SRC)/main.c

all: clean $(EXE) run

$(EXE): $(SRC)/main.c
	$(CC) $(C_FILES) -o $(EXE).out

run:
	./$(EXE).out $(IN) | tee $(SAIDA_LOG)

clean:
	rm -f Logs/*.txt
	rm -f *.out


.PHONY:  run clean