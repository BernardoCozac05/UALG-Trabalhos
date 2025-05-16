#include <stdio.h>

void initArray() {
    int array[3];
    int i = 3, x = 3;

    while (--i && (array[i] = --x))
        printf("%d ", array[i]);
}

int main() {
    initArray();
}

///////Answer is "2 1"