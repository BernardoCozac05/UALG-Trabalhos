/* Sistemas Operativos 2005/06
 * Data: 10/Out/2005
 * Autor: Margarida Madeira e Moura
 * Objectivo: Identificador de processo e processo pai
 * Compilar com: gcc -Wall -o prog1 prog1.c
 * Executar com: ./prog1
 */

#include <stdio.h>
#include <unistd.h>


int main(void)
{
	printf("Sou um Processo.\n");
	printf("Sou o processo %d \n", getpid());
	printf("Fui criado pelo processo %d \n", getppid());

	// getchar();

	return(0);
}