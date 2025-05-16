#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAX_COMMAND_LEN 30

// Estrutura para mapear o nome dos comandos com funções
typedef struct {
    char *name;
    void (*func)(Stack *, char *);
} Command;

// Protótipos das funções
void handle_push(Stack *s, char *arg);
void handle_pop(Stack *s, char *arg);
void handle_top(Stack *s, char *arg);

// Implementações
void handle_push(Stack *s, char *arg) {
    if (arg == NULL) {
        printf("Error: No value provided for push.\n");
        return;
    }
    int value = atoi(arg);
    push(s, value);
}

void handle_pop(Stack *s, char *arg) {
    (void)arg; // evitar warning de parâmetro não utilizado
    pop(s);
}

void handle_top(Stack *s, char *arg) {
    (void)arg;
    top(s);
}

// Vetor que mapeia comandos a funções concretas
Command commands[] = {
    {"push", handle_push},
    {"pop", handle_pop},
    {"top", handle_top}
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int main() {
    Stack stack;
    init(&stack);

    char input[MAX_COMMAND_LEN];

    // Lê até EOF
    while (fgets(input, MAX_COMMAND_LEN, stdin) != NULL) {
        // Remove o caractere de nova linha
        input[strcspn(input, "\n")] = '\0';

        // Divide em comando e argumento
        char *command = strtok(input, " ");
        char *arg = strtok(NULL, " ");

        if (command == NULL)
            continue;

        int found = 0;

        for (int i = 0; i < COMMAND_COUNT; i++) {
            if (strcmp(command, commands[i].name) == 0) {
                found = 1;
                commands[i].func(&stack, arg);  // Executa função com argumento
                break;
            }
        }

        if (!found) {
            printf("Comando desconhecido: %s\n", command);
        }
    }

    return 0;
}
