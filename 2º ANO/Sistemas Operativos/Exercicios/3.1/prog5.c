/* Sistemas Operativos 2025/26
 * Data: 17/Mar/2026
 * Autor: Diogo Carvalho
 * Objectivo: Distinguir processo pai de processo filho após fork()
 * Compilar com: gcc -Wall -o prog5 prog5.c
 * Executar com: ./prog5
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid;

    printf("Sou um Processo.\n");
    printf("Antes do fork: processo %d, filho de %d\n", getpid(), getppid());

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        printf("Processo Filho: PID = %d, PPID = %d\n", getpid(), getppid());
    }
    else {
        printf("Processo Pai: PID = %d, criou o filho com PID = %d\n", getpid(), pid);
    }

    return 0;
}