#include <stdio.h>
#include <string.h>
// Função legítima
void funcao_normal() {
printf("Função normal executada.\n");
}
// Função maliciosa
void funcao_maliciosa() {
printf(" Código malicioso executado! \n");
}
int main(void) {

void (*ptr_func)() = funcao_normal; // Apontador de função legítimo
char buffer[8];

// Simula um buffer overflow que sobrescreve o apontador de função (definido "ao lado" de buffer)

strcpy(buffer, "AAAAAAAAAAAAAAAAAAA");

// note que estamos a copiar para buffer dados em excesso, daí o Overflow
// maliciosamente bem definido, este Overflow pode modificar ptr_func!
printf("Chamando a função:\n");
ptr_func(); // Se o overflow modificar ptr_func, pode chamar funcao_maliciosa!

return 0;
}