#include <stdio.h>
#include <string.h>  // para memcpy

// Função genérica de bubble sort
void bubbleSort(void* array, int n, size_t size, int (*cmp)(const void*, const void*)) {
    char* arr = (char*)array;
    char temp[size];

    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
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

// Função de comparação para inteiros
int cmpInt(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    return (ia > ib) - (ia < ib);  // ou: return ia - ib;
}

// Função de comparação para floats
int cmpFloat(const void* a, const void* b) {
    float fa = *(float*)a;
    float fb = *(float*)b;
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
        printf("%.1f ", arr[i]);
    printf("\n");
}

int main() {
    //int numeros[] = {8, 7, 32, 8, 6, 11, 9};
    //float reais[] = {7.0, 8.0, 4.0, -13.1, 67.3, 9.7, 4.1, -1.4};
    int numeros[] = {42, 7, 91, 13, 65, 28, 84, 3, 76, 39, 50, 12, 97, 6, 71, 24, 59, 11, 36, 88};
    float reais[] = {9.81, -3.5, 0.99, 4.2, 7.77, -1.23, 12.6, 5.5, 3.14, 8.88, 11.11, 1.01, -7.4, 6.66, 2.71, 0.01, 10.5, -2.5, 13.37, 0.0};
    

    int sizeInt = sizeof(numeros) / sizeof(numeros[0]);
    int sizeFloat = sizeof(reais) / sizeof(reais[0]);

    // Antes da ordenação
    printIntArray(numeros, sizeInt);
    bubbleSort(numeros, sizeInt, sizeof(int), cmpInt);
    printIntArray(numeros, sizeInt);

    printFloatArray(reais, sizeFloat);
    bubbleSort(reais, sizeFloat, sizeof(float), cmpFloat);
    printFloatArray(reais, sizeFloat);

    return 0;
}
