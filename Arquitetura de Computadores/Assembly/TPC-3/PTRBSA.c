#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para ordenar um array de inteiros com bubble sort
void bubbleSortInt(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Função para ordenar um array de floats com bubble sort
void bubbleSortFloat(float arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                float temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Função para imprimir um array de inteiros
void printIntArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Função para imprimir um array de floats
void printFloatArray(float arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%.2f ", arr[i]);
    printf("\n");
}

// Função para ler inteiros de uma linha
int* lerInteiros(int* count) {
    char linha[1024];
    int capacidade = 10;
    int* array = malloc(capacidade * sizeof(int));
    *count = 0;

    printf("Insira inteiros separados por espaço:\n");
    fgets(linha, sizeof(linha), stdin);

    char* token = strtok(linha, " \t\n");
    while (token != NULL) {
        if (*count >= capacidade) {
            capacidade *= 2;
            array = realloc(array, capacidade * sizeof(int));
        }
        array[*count] = strtol(token, NULL, 10);
        (*count)++;
        token = strtok(NULL, " \t\n");
    }

    return array;
}

// Função para ler floats de uma linha
float* lerFloats(int* count) {
    char linha[1024];
    int capacidade = 10;
    float* array = malloc(capacidade * sizeof(float));
    *count = 0;

    printf("Insira floats separados por espaço:\n");
    fgets(linha, sizeof(linha), stdin);

    char* token = strtok(linha, " \t\n");
    while (token != NULL) {
        if (*count >= capacidade) {
            capacidade *= 2;
            array = realloc(array, capacidade * sizeof(float));
        }
        array[*count] = strtof(token, NULL);
        (*count)++;
        token = strtok(NULL, " \t\n");
    }

    return array;
}

int main() {
    int sizeInt, sizeFloat;

    // Ler inteiros
    int* numeros = lerInteiros(&sizeInt);
    bubbleSortInt(numeros, sizeInt);
    printIntArray(numeros, sizeInt);

    // Ler floats
    float* reais = lerFloats(&sizeFloat);
    bubbleSortFloat(reais, sizeFloat);
    printFloatArray(reais, sizeFloat);

    free(numeros);
    free(reais);

    return 0;
}
