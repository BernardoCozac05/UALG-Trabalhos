#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função genérica de bubble sort
void bubbleSort(void* array, int n, size_t size, int (*cmp)(const void*, const void*)) {
    char* arr = (char*)array;
    char temp[size];

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            void* elem1 = arr + j * size;
            void* elem2 = arr + (j + 1) * size;

            if (cmp(elem1, elem2) > 0) {
                memcpy(temp, elem1, size);
                memcpy(elem1, elem2, size);
                memcpy(elem2, temp, size);
            }
        }
    }
}

// Comparador para inteiros
int cmpInt(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Comparador para floats
int cmpFloat(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
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

    // Ler e ordenar inteiros
    int* numeros = lerInteiros(&sizeInt);
    bubbleSort(numeros, sizeInt, sizeof(int), cmpInt);
    printIntArray(numeros, sizeInt);

    // Ler e ordenar floats
    float* reais = lerFloats(&sizeFloat);
    bubbleSort(reais, sizeFloat, sizeof(float), cmpFloat);
    printFloatArray(reais, sizeFloat);

    free(numeros);
    free(reais);

    return 0;
}
