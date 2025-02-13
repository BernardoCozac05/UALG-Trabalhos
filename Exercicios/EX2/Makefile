CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/counter.c
OBJ = counter.o
OUT = libcounter

all: static shared

static: $(OBJ)
	ar rcs libcounter.a $(OBJ)

shared: $(OBJ)
	$(CC) -shared -o libcounter.so $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

test: shared
	$(CC) $(CFLAGS) test/main.c -L. -lcounter -o test_counter
	./test_counter.exe
####LD_LIBRARY_PATH=. ./test_counter (MAC/IOS)

clean:

	del /F /Q counter.o libcounter.a libcounter.so test_counter.exe 2> NUL

##clean:
####rm -f $(OBJ) libcounter.a libcounter.so test_counter (MAC/IOS)
