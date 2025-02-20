CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =
SRC = src/div.c src/main.c
OBJ = src/div.o src/main.o
OUT = div_program.exe

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	-$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG_ON" all

clean:
	-if exist src\\div.o del /Q src\\div.o
	-if exist src\\main.o del /Q src\\main.o
	-if exist $(OUT) del /Q $(OUT)
