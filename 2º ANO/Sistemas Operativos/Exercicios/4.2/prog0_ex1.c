/* Sistemas Operativos 2025/26
 * Data: 24/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Criar duas threads, uma para a funcao um e outra para a funcao dois
 * Compilar com: gcc -Wall -pthread -o prog0_ex1 prog0_ex1.c
 * Executar com: ./prog0_ex1
 */

#include <stdio.h>
#include <pthread.h>

void *um(void *arg)
{
	printf("Funcao um\n");
	return NULL;
}

void *dois(void *arg)
{
	printf("Funcao dois\n");
	return NULL;
}

int main(void)
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, um, NULL);
	pthread_create(&t2, NULL, dois, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}