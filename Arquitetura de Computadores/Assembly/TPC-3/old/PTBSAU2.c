#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Protótipo de bsort:  
// - base:   ponteiro para o início do array (qualquer tipo)
// - nitems: número de elementos
// - size:   tamanho, em bytes, de cada elemento
// - cmp:    função de comparação (retorna <0, 0 ou >0)
void bsort(void *base, int nitems, size_t size, int (*cmp)(const void *, const void *))
{
    char *arr = base;
    char *tmp = malloc(size);
    if (!tmp) {
        fprintf(stderr, "Erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    int change;
    do {
        change = 0;
        for (int i = 0; i < nitems - 1; i++) {
            void *p = arr + i * size;
            void *q = arr + (i + 1) * size;
            if (cmp(p, q) > 0) {
                // troca p <-> q
                memcpy(tmp, p, size);
                memcpy(p, q, size);
                memcpy(q, tmp, size);
                change = 1;
            }
        }
    } while (change);

    free(tmp);
}

// Função de comparação para inteiros
int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

// Função de comparação para floats
int cmp_float(const void *a, const void *b) {
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    if (fa < fb) return -1;
    if (fa > fb) return  1;
    return 0;
}

// Imprime array de inteiros
void print_int(int v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

// Imprime array de floats
void print_float(float v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%.2f ", v[i]);
    printf("\n");
}

int main(void) {
    int   numeros[] = {8, 7, 32, 8, 6, 11, 9};
    float reais[]   = {7.0, 8.0, 4.0, -13.1, 67.3, 9.7, 4.1, -1.4};

    int n_num   = sizeof(numeros) / sizeof(numeros[0]);
    int n_reais = sizeof(reais)   / sizeof(reais[0]);

    // --- Inteiros ---
    print_int(numeros, n_num);

    bsort(numeros, n_num, sizeof(int), cmp_int);

    print_int(numeros, n_num);

    // --- Floats ---
    print_float(reais, n_reais);

    bsort(reais, n_reais, sizeof(float), cmp_float);

    print_float(reais, n_reais);

    return 0;
}
