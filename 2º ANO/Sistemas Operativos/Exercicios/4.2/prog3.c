/* Sistemas Operativos 2005/06
 * Data: 10/Out/2005
 * Autor: Margarida Madeira e Moura
 * Objectivo: Directórios
 * Compilar com: gcc -Wall -o prog3 prog3.c
 * Executar com: ./prog3
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int Estado;
char *acima = "../";
char actual[255];

int main(void)
{
	getcwd(actual, 255);
	printf("Ponto de partida: %s \n \n", actual);
	do {
		Estado=chdir(acima);
		getcwd(actual, 255);
		printf("\t Ponto actual: %s \n", actual);
		getchar();

	} while ((!Estado) && (strlen(actual) > 1) );

	if (Estado) perror((void *) &Estado);
	
	return(0);
}