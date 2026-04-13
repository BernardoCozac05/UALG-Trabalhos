/* Sistemas Operativos 2005/06
 * Data: 10/Out/2005
 * Autor: Margarida Madeira e Moura
 * Objectivo: Identificador de processo, processo pai e utilizador
 * Compilar com: gcc -Wall -o prog2 prog2.c
 * Executar com: ./prog2
 */

#include <stdio.h>
#include <unistd.h>


int main(void)
{
	printf("Sou um Processo.\n");
	printf("Sou o processo %d \n", getpid());
	printf("Fui criado pelo processo %d , a pedido do utilizador %d \n", getppid(), getuid());


	// getchar();

	return(0);
}