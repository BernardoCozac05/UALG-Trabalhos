#include <stdio.h>
#include "utils.h" // declaramos querer usar um serviço muito simples
// utilização de um serviço muito simples

int main() {
    printf("Hello, World!\n");
    int result = add(3, 5);
    printf("3 + 5 = %d\n", result);
    printf("\033[32mTEST 1 PASS\033[0m\n");
    int power = powerOf(7,8);
    int mult = multiply(10,10);
    printf("7 ^ 8 = %d\n", power);
    printf("\033[32mTEST 2 PASS\033[0m\n");
    printf("10 * 10 = %d\n", mult);
    printf("\033[32mTEST 3 PASS\033[0m\n");
    return 0;
}