/* Sistemas Operativos 2025/26
 * Data: 24/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Subir na arvore de directorios e listar o directorio corrente em lock step
 * Compilar com: gcc -Wall -pthread -o prog3_ex5 prog3_ex5.c
 * Executar com: ./prog3_ex5
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <semaphore.h>

int fim = 0;
char *acima = "../";
char actual[255];
sem_t pode_subir;
sem_t pode_listar;

void listar_directorio(void)
{
	DIR *d;
	struct dirent *ent;

	d = opendir(".");
	if (d == NULL) {
		perror("opendir");
		return;
	}

	printf("Conteudo de %s\n", actual);
	while ((ent = readdir(d)) != NULL) {
		printf("\t%s\n", ent->d_name);
	}
	printf("\n");

	closedir(d);
}

void *tarefa_sobe(void *arg)
{
	do {
		sem_wait(&pode_subir);

		chdir(acima);
		getcwd(actual, 255);
		printf("Ponto actual: %s\n\n", actual);

		if (strlen(actual) <= 1)
			fim = 1;

		sem_post(&pode_listar);

	} while (!fim);

	return NULL;
}

void *tarefa_lista(void *arg)
{
	do {
		sem_wait(&pode_listar);

		listar_directorio();

		if (!fim) {
			getchar();
			sem_post(&pode_subir);
		}

	} while (!fim);

	return NULL;
}

int main(void)
{
	pthread_t t1, t2;

	getcwd(actual, 255);
	printf("Ponto de partida: %s\n\n", actual);

	sem_init(&pode_subir, 0, 1);
	sem_init(&pode_listar, 0, 0);

	pthread_create(&t1, NULL, tarefa_sobe, NULL);
	pthread_create(&t2, NULL, tarefa_lista, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&pode_subir);
	sem_destroy(&pode_listar);

	return 0;
}