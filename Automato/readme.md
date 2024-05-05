## Manual de compilação e execução.

Para executar o código:

```
make all
```

Para adicionar um novo arquivo no código: em *C_FILES*, colocar um *\\* ao lado do ultimo .c e adicionar seu arquivo na proxima linha.

```
C_FILES := \
	$(SRC)/arquivo_da_lista.c \
    $(SRC)/seu_arquivo.c
```