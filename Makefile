CC = gcc
SRC := src
EXE := analisador_lexico.o

C_FILES := \
	$(SRC)/tabelas.c \
	$(SRC)/main.c

all: clean $(EXE) run

$(EXE): $(SRC)/main.c $(SRC)/analisador_lexico.c
	$(CC) $(C_FILES) -o $(EXE)

run:
	./$(EXE)

clean:
	rm -f *.o
