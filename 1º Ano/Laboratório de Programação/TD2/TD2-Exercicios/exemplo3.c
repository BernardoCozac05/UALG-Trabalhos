#include <stdio.h>
int soma (int a, int b) {return a + b;}
int subtrai (int a, int b) {return a - b;}
int multiplica(int a, int b) {return a * b;}
int divide (int a, int b) {return (b)? a / b : 0;}
int main(void) {
    
// Vetor de apontadores para funções. Cada função tem a assinatura int (int, int)
int (*operacoes[4])(int, int) = { soma, subtrai, multiplica, divide };

int a = 20, b = 5;

// Usamos o vetor de funções para executar as operações
printf("soma(%d, %d) = %d\n", a, b, operacoes[0](a, b));
printf("subtrai(%d, %d) = %d\n", a, b, operacoes[1](a, b));
printf("multiplica(%d, %d) = %d\n", a, b, operacoes[2](a, b));
printf("divide(%d, %d) = %d\n", a, b, operacoes[3](a, b));
return 0;
}