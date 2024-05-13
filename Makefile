CC = gcc
SRC := Source
EXE := analisador_lexico
SAIDA := Logs/log.txt

C_FILES := \
	$(SRC)/tabelas.c \
	$(SRC)/funcoes_saida.c \
	$(SRC)/analisador_lexico.c \
	$(SRC)/main.c

all: clean $(EXE) run

$(EXE): $(SRC)/main.c
	$(CC) $(C_FILES) -o $(EXE).o

run:
	./$(EXE).o $(IN) | tee $(SAIDA)

clean:
	rm -f *.o
	rm -f Logs/*.txt
