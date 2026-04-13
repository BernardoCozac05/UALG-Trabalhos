/* Sistemas Operativos 2025/26
 * Data: 24/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Tentar inverter a ordem de impressao das mensagens
 * Compilar com: gcc -Wall -pthread -o prog0_ex3 prog0_ex3.c
 * Executar com: ./prog0_ex3
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *um(void *arg)
{
	sleep(1);
	printf("Funcao um: PID=%d PPID=%d\n", getpid(), getppid());
	return NULL;
}

void *dois(void *arg)
{
	printf("Funcao dois: PID=%d PPID=%d\n", getpid(), getppid());
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