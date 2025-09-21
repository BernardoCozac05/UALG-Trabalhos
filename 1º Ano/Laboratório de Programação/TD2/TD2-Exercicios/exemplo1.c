#include <stdio.h>
// Função que recebe dois inteiros e retorna um inteiro

int soma(int a, int b) {
    return a + b;
}

int main(void) {
    // Apontador de função com assinatura errada (deveria ser i(*)(int, int))

    int (*ptr)(int) = (int (*)(int)) soma; // Casting inadequado! e o compilador não se queixa!

    // Chamamos a função incorretamente e o compilador não se queixa

    printf("%d\n", ptr(10)); // Comportamento indefinido!
    return 0;
    }