/* Sistemas Operativos 2025/26
 * Data: 24/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Implementar o problema do Jantar dos Filosofos com N filosofos
 * Compilar com: gcc -Wall -pthread -o jantar jantar.c
 * Executar com: ./jantar 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int n;
sem_t *garfos;
sem_t sala;

void pensar(int i)
{
	printf("Filosofo %d a pensar\n", i);
	sleep(1);
}

void comer(int i)
{
	printf("Filosofo %d a comer\n", i);
	sleep(1);
}

void *filosofo(void *arg)
{
	int i = *((int *) arg);
	int esquerda = i;
	int direita = (i + 1) % n;

	while (1) {
		pensar(i);

		sem_wait(&sala);
		sem_wait(&garfos[esquerda]);
		sem_wait(&garfos[direita]);

		comer(i);

		sem_post(&garfos[direita]);
		sem_post(&garfos[esquerda]);
		sem_post(&sala);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t *t;
	int *id;

	if (argc != 2) {
		printf("Uso: %s numero_de_filosofos\n", argv[0]);
		return 1;
	}

	n = atoi(argv[1]);

	if (n < 2) {
		printf("O numero de filosofos deve ser >= 2\n");
		return 1;
	}

	t = (pthread_t *) malloc(n * sizeof(pthread_t));
	id = (int *) malloc(n * sizeof(int));
	garfos = (sem_t *) malloc(n * sizeof(sem_t));

	if ((t == NULL) || (id == NULL) || (garfos == NULL)) {
		printf("Erro na alocacao de memoria\n");
		return 1;
	}

	sem_init(&sala, 0, n - 1);

	for (i = 0; i < n; i++)
		sem_init(&garfos[i], 0, 1);

	for (i = 0; i < n; i++) {
		id[i] = i;
		pthread_create(&t[i], NULL, filosofo, &id[i]);
	}

	for (i = 0; i < n; i++)
		pthread_join(t[i], NULL);

	for (i = 0; i < n; i++)
		sem_destroy(&garfos[i]);

	sem_destroy(&sala);

	free(t);
	free(id);
	free(garfos);

	return 0;
}