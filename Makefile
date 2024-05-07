CC = gcc
SRC := src
EXE := analisador_lexico.o
SAIDA := log.txt

C_FILES := \
	$(SRC)/tabelas.c \
	$(SRC)/funcoes_saida.c \
	$(SRC)/main.c

all: clean $(EXE) run

$(EXE): $(SRC)/main.c
	$(CC) $(C_FILES) -o $(EXE)

run:
	./$(EXE) | tee $(SAIDA)

clean:
	rm -f *.o
