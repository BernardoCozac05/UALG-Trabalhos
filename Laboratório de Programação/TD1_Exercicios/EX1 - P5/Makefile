# Que compilador usar?
CC = gcc
# Com que opções?
CFLAGS = -Wall -Wextra
# Com que nome de executável?
OUT = magia
# Quais são os ficheiros fontes?
SRCS = main.c utils.c
# Ficheiros objetos
OBJS = $(SRCS:.c=.o)

# Diretiva por omissão: construir o executável
all: $(OUT)

# Diretiva para criar o executável
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

# Compilar os ficheiros .c para ficheiros objetos .o
%.o: %.c utils.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar os ficheiros caso se pretenda reiniciar a compilação do projeto.
clean:
	rm -f $(OBJS) $(OUT)
