#include <stdio.h>
#include <stdlib.h>


void push(int* stack, int* top, int value) {
    stack[++(*top)] = value;  // Incrementa top e adiciona o valor
}

int pop(int* stack, int* top) {
    return stack[(*top)--];  // Retorna o topo e decrementa
}


// Função auxiliar: maior área retangular no histograma (larguras variáveis)
int largestRectangleArea(int* widths, int* heights, int n) {
    int *stack = malloc(n * sizeof(int));       // pilha de índices
    int *prefixWidth = malloc((n + 1) * sizeof(int)); // soma acumulada das larguras

    if (!stack || !prefixWidth) {
        // Falha de alocação: liberar e retornar 0 (imprime 0 fora)
        free(stack);
        free(prefixWidth);
        return 0;
    }

    prefixWidth[0] = 0;
    for (int i = 0; i < n; i++) {
        prefixWidth[i + 1] = prefixWidth[i] + widths[i];
    }

    int maxArea = 0;
    int top = -1;

    // Passa por todas as caixas + 1 sentinela (i==n => altura=0)
    for (int i = 0; i <= n; i++) {
        // A altura é 0 quando i == n, forçando desempilhar tudo ao final
        int currHeight = (i == n) ? 0 : heights[i];

        // Aqui mudamos de '>=' para '>' para lidar melhor com casos TSame
        while (top >= 0 && heights[stack[top]] > currHeight) {
            int h = heights[pop(stack, &top)];
            int width = (top < 0)
                ? prefixWidth[i]
                : prefixWidth[i] - prefixWidth[stack[top] + 1];
            int area = h * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
        push(stack, &top, i);
    }

    free(stack);
    free(prefixWidth);
    return maxArea;
}

int main(void) {
    int capacity = 16;
    int *heights = malloc(capacity * sizeof(int));
    int *widths  = malloc(capacity * sizeof(int));
    if (!heights || !widths) {
        // Falha alocando de início => só imprime 0 e sai
        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    int n = 0;
    // Ler do stdin até EOF (sem mensagens de erro)
    while (1) {
        int w, h;
        if (scanf("%d %d", &w, &h) != 2) {
            // Se não leu 2 ints, acabou ou leitura inválida => parar
            break;
        }
        // Se quiser ignorar entradas inválidas:
        // if (w <= 0 || h < 0) continue;

        // Realocar se preciso
        if (n == capacity) {
            capacity *= 2;
            int *newH = realloc(heights, capacity * sizeof(int));
            int *newW = realloc(widths, capacity * sizeof(int));
            if (!newH || !newW) {
                free(heights);
                free(widths);
                printf("0\n");
                return 0;
            }
            heights = newH;
            widths  = newW;
        }
        widths[n]  = w;
        heights[n] = h;
        n++;
    }

    // Se não leu nada, imprime 0
    if (n == 0) {
        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    // Calcular a maior área
    int maxArea = largestRectangleArea(widths, heights, n);

    // Imprimir resultado final
    printf("%d\n", maxArea);

    free(heights);
    free(widths);
    return 0;  // sucesso
}
