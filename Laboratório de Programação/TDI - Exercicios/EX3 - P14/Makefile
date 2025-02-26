CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =
SRC = src/div.c src/main.c
OBJ = $(SRC:.c=.o)
OUT = div_program.exe  # Adicionando extensão .exe para Windows

all: build

build: $(OBJ)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -DDEBUG_ON
debug: build

clean:
	-del /Q src\div.o 2>nul || exit 0
	-del /Q src\main.o 2>nul || exit 0
	-del /Q $(OUT) 2>nul || exit 0
