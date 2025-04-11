#include <stdio.h>
#include <stdlib.h>

// Função para empilhar um elemento: incrementa o índice do topo e armazena o valor na pilha.
void push(int* stack, int* top, int value) {
    stack[++(*top)] = value;
}

// Função para desempilhar um elemento: retorna o valor no topo e decrementa o índice do topo.
int pop(int* stack, int* top) {
    return stack[(*top)--];
}

// Função que calcula a maior área retangular em um histograma com larguras variáveis.
// Recebe dois arrays (widths e heights) e o número de barras (n).
int largestRectangleArea(int* widths, int* heights, int n) {
    // Aloca memória para a pilha de índices e para o array que guarda a soma acumulada das larguras.
    int *stack = malloc(n * sizeof(int));
    int *prefixWidth = malloc((n + 1) * sizeof(int));

    // Verifica se a alocação falhou; em caso afirmativo, libera os recursos e retorna 0.
    if (!stack || !prefixWidth) {
        
        free(stack);
        free(prefixWidth);
        return 0;
    }

    // Inicializa o array prefixWidth com a soma acumulada das larguras.
    prefixWidth[0] = 0;
    for (int i = 0; i < n; i++) {
        prefixWidth[i + 1] = prefixWidth[i] + widths[i];
    }

    int maxArea = 0; // Variável para armazenar a maior área encontrada.
    int top = -1;   // Inicializa a pilha vazia (índice -1 indica pilha vazia).

    // Itera sobre todas as barras e adiciona uma barra sentinela com altura 0 no final para forçar o desempilhamento.
    for (int i = 0; i <= n; i++) {
        // Define a altura atual: se for o índice n (sentinela), a altura é 0.
        int currHeight = (i == n) ? 0 : heights[i];

        // Enquanto houver barras na pilha e a altura da barra no topo for maior que a altura atual,
        // desempilha e calcula a área potencial com a altura da barra desempilhada.
        while (top >= 0 && heights[stack[top]] > currHeight) {
            // Retira o índice do topo e obtém a altura correspondente.
            int h = heights[pop(stack, &top)];
            // Calcula a largura correspondente à barra desempilhada usando o array de prefixos.
            int width = (top < 0)
                ? prefixWidth[i]
                : prefixWidth[i] - prefixWidth[stack[top] + 1];
            int area = h * width;  // Calcula a área do retângulo.
            if (area > maxArea) {
                maxArea = area;  // Atualiza a maior área, se necessário.
            }
        }
        // Empilha o índice atual para continuar o processamento.
        push(stack, &top, i);
    }

    // Libera a memória alocada e retorna a maior área encontrada.
    free(stack);
    free(prefixWidth);
    return maxArea;
}

int main(void) {
    int capacity = 16; // Capacidade inicial dos arrays que armazenarão as larguras e alturas.
    int *heights = malloc(capacity * sizeof(int));  // Aloca memória para armazenar as alturas.
    int *widths  = malloc(capacity * sizeof(int));  // Aloca memória para armazenar as larguras.
    
    // Verifica se a alocação inicial foi bem-sucedida; se não, libera os recursos e imprime 0.
    if (!heights || !widths) {

        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    int n = 0;  // Contador para o número de barras lidas.
    // Loop para ler pares de inteiros (largura e altura) da entrada padrão até que não seja mais possível ler.
    while (1) {
        int w, h;
        if (scanf("%d %d", &w, &h) != 2) {
            break;  // Encerra o loop se não for possível ler dois inteiros.
        }


        // Se o array estiver cheio, duplica a capacidade e realoca os arrays.
        if (n == capacity) {
            capacity *= 2;
            int *newH = realloc(heights, capacity * sizeof(int));
            int *newW = realloc(widths, capacity * sizeof(int));
            if (!newH || !newW) {
                free(newH ? newH : heights);
                free(newW ? newW : widths);
                printf("0\n");
                return 0;
            }
            heights = newH;
            widths  = newW;
            
        }
        // Armazena os valores lidos nos arrays correspondentes.
        widths[n]  = w;
        heights[n] = h;
        n++;
    }

    // Se nenhum par de valores foi lido, libera a memória e imprime 0.
    if (n == 0) {
        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    // Calcula a maior área retangular utilizando a função largestRectangleArea.
    int maxArea = largestRectangleArea(widths, heights, n);

    // Imprime o resultado final.
    printf("%d\n", maxArea);

    // Libera a memória alocada para os arrays e encerra o programa.
    free(heights);
    free(widths);
    return 0;
}
