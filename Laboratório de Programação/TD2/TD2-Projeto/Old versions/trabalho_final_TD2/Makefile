CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/palavra.c src/dynvec.c src/main.c
OBJ = $(SRC:.c=.o) 
TARGET = exercicio.exe
TESTES = $(wildcard testes/t*.txt)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c include/palavra.h include/dynvec.h
	$(CC) $(CFLAGS) -c $< -o $@

testes: $(TARGET)
	@for file in $(TESTES); do \
		echo "===========================Executando o teste $$file ==============================="; \
		./$(TARGET) $$file; \
		echo "============================================================================================="; \
	done

clean:
	rm -f $(OBJ) $(TARGET)

fclean:
	rm -f $(TARGET)
