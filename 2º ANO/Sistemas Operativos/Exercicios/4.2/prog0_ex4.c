/* Sistemas Operativos 2025/26
 * Data: 24/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Sincronizar a ordem de impressao das mensagens com semaforos
 * Compilar com: gcc -Wall -pthread -o prog0_ex4 prog0_ex4.c
 * Executar com: ./prog0_ex4
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t ordem;

void *um(void *arg)
{
	sem_wait(&ordem);
	printf("Funcao um: PID=%d PPID=%d\n", getpid(), getppid());
	return NULL;
}

void *dois(void *arg)
{
	printf("Funcao dois: PID=%d PPID=%d\n", getpid(), getppid());
	sem_post(&ordem);
	return NULL;
}

int main(void)
{
	pthread_t t1, t2;

	sem_init(&ordem, 0, 0);

	pthread_create(&t1, NULL, um, NULL);
	pthread_create(&t2, NULL, dois, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&ordem);

	return 0;
}