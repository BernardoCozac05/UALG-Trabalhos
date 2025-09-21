#include <stdio.h>

// Função para ordenar um array de inteiros com bubble sort
void bubbleSortInt(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                // troca os elementos
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
                // troca os elementos
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
        printf("%.1f ", arr[i]);
    printf("\n");
}

int main() {
    int numeros[] = {8, 7, 32, 8, 6, 11, 9};
    float reais[] = {7.0, 8.0, 4.0, -13.1, 67.3, 9.7, 4.1, -1.4};
    //int numeros[] = {42, 7, 91, 13, 65, 28, 84, 3, 76, 39, 50, 12, 97, 6, 71, 24, 59, 11, 36, 88};
    //float reais[] = {9.81, -3.5, 0.99, 4.2, 7.77, -1.23, 12.6, 5.5, 3.14, 8.88, 11.11, 1.01, -7.4, 6.66, 2.71, 0.01, 10.5, -2.5, 13.37, 0.0};
    
    // Calcula o tamanho dos arrays
    int sizeInt = sizeof(numeros) / sizeof(numeros[0]);
    int sizeFloat = sizeof(reais) / sizeof(reais[0]);

    // Imprime os arrays antes da ordenação
    printIntArray(numeros, sizeInt);
    // Ordenar com bubble sort
    bubbleSortInt(numeros, sizeInt);
    // Imprime os arrays após a ordenação
    printIntArray(numeros, sizeInt);


    // Imprime os arrays antes da ordenação
    printFloatArray(reais, sizeFloat);
    // Ordenar com bubble sort
    bubbleSortFloat(reais, sizeFloat);
    // Imprime os arrays após a ordenação
    printFloatArray(reais, sizeFloat);

    return 0;
}
