/* Sistemas Operativos 2005/06
 * Data: 10/Out/2005
 * Autor: Margarida Madeira e Moura
 * Objectivo: Criar processos
 * Compilar com: gcc -Wall -o prog4 prog4.c
 * Executar com: ./prog4
 */

#include <stdio.h>
#include <unistd.h>


int main(void)
{
	printf("Sou um Processo.\n");
	printf("Sou o processo %d, filho de %d \n", getpid(), getppid());

	printf("Bom dia ... \n");

	fork();
	
	printf("a todos \n");
	//printf("Sou o processo %d, filho de %d \n", getpid(), getppid());

	return(0);
}