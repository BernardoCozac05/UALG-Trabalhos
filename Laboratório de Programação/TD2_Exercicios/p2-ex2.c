#include <stdio.h>
#include <stdlib.h>
// exercicio 2,pag 34

void aloca_memoria() {
    int* k = malloc(sizeof(int));
    printf("endereço de k: %p\n", k);
}

int main(void) {
    int a = 10;
    printf("endereço de a: %p\n", &a);

    int* i = malloc(sizeof(int));

    (*i) = 55;
    printf("endereço de i: %p\n", i);

    aloca_memoria();
    return 0;

}