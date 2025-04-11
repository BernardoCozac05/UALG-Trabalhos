#include <stdio.h>
#include <stdlib.h>

// Insere um valor na pilha, incrementando o índice do topo e colocando o valor na nova posição.
void push(int* stack, int* top, int value) {
    stack[++(*top)] = value;
}

// Remove e retorna o valor do topo da pilha, decrementando o índice do topo.
int pop(int* stack, int* top) {
    return stack[(*top)--];
}

// Calcula a maior área retangular possível em um histograma onde cada barra possui largura e altura variáveis.
// Parâmetros:
//   widths: vetor contendo as larguras de cada barra
//   heights: vetor contendo as alturas de cada barra
//   n: número total de barras do histograma
int largestRectangleArea(int* widths, int* heights, int n) {
    // Aloca um vetor para ser usado como pilha que armazena os índices das barras.
    int *stack = malloc(n * sizeof(int));
    // Aloca um vetor para armazenar as somas prefixadas das larguras, facilitando o cálculo das larguras acumuladas.
    int *prefixWidth = malloc((n + 1) * sizeof(int));

    // Se a alocação de memória falhar, libera os recursos e retorna 0.
    if (!stack || !prefixWidth) {

        free(stack);
        free(prefixWidth);
        return 0;
    }

    // Inicializa o vetor de somas prefixadas: prefixWidth[0] começa em 0, e cada posição seguinte é a soma acumulada das larguras.
    prefixWidth[0] = 0;
    for (int i = 0; i < n; i++) {
        prefixWidth[i + 1] = prefixWidth[i] + widths[i];
    }

    int maxArea = 0;  // Variável para armazenar a maior área encontrada.
    int top = -1;     // Inicializa a pilha como vazia (topo = -1).

    // Itera por todas as barras do histograma e por uma iteração extra para processar os elementos remanescentes na pilha.
    for (int i = 0; i <= n; i++) {
        // Define a altura atual: se i == n, utiliza 0 como valor sentinela; caso contrário, a altura da barra corrente.
        int currHeight = (i == n) ? 0 : heights[i];

        // Enquanto a pilha não estiver vazia e a altura da barra no topo da pilha for maior que a altura atual,
        // processa a barra removendo-a da pilha e calculando a área retangular que pode ser formada.
        while (top >= 0 && heights[stack[top]] > currHeight) {
            int h = heights[pop(stack, &top)]; // Remove a barra do topo e obtém sua altura.
            // Calcula a largura do retângulo:
            // Se a pilha estiver vazia após a remoção, a largura é a soma acumulada até a posição i.
            // Caso contrário, a largura é a diferença entre a soma acumulada atual e a soma acumulada logo após o novo topo.
            int width = (top < 0) ? prefixWidth[i] : prefixWidth[i] - prefixWidth[stack[top] + 1];
            int area = h * width;  // Calcula a área com base na altura e na largura obtida.
            // Atualiza a maior área encontrada, se o valor atual for maior.
            if (area > maxArea) {
                maxArea = area;
            }
        }
        // Insere o índice da barra atual na pilha para análise futura.
        push(stack, &top, i);
    }

    // Libera a memória alocada para a pilha e para o vetor de somas prefixadas.
    free(stack);
    free(prefixWidth);
    // Retorna a maior área retangular encontrada.
    return maxArea;
}

int main(void) {
    // Define a capacidade inicial para armazenar os dados das barras.
    int capacity = 16;
    // Aloca memória para os vetores de alturas e larguras das barras.
    int *heights = malloc(capacity * sizeof(int));
    int *widths  = malloc(capacity * sizeof(int));

    // Verifica se a alocação falhou; se sim, libera os recursos, imprime 0 e encerra o programa.
    if (!heights || !widths) {
        
        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    int n = 0;  // Contador para o número de barras lidas.
    // Lê indefinidamente pares de inteiros (largura e altura) da entrada padrão.
    while (1) {
        int w, h;
        // Se não for possível ler dois inteiros, encerra a leitura.
        if (scanf("%d %d", &w, &h) != 2) {
            break;
        }

        // Se o número de barras atingir a capacidade do vetor, dobra a capacidade e realoca os vetores.
        if (n == capacity) {
            capacity *= 2;
            int *newH = realloc(heights, capacity * sizeof(int));
            int *newW = realloc(widths, capacity * sizeof(int));

            // Se a realocação falhar, libera os recursos alocados (se houver) e encerra o programa imprimindo 0.
            if (!newH || !newW) {
                free(newH ? newH : heights);
                free(newW ? newW : widths);
                printf("0\n");
                return 0;
            }
            heights = newH;
            widths  = newW;
        }
        // Armazena os valores lidos nos vetores correspondentes.
        widths[n]  = w;
        heights[n] = h;
        n++;  // Incrementa o contador de barras.
    }

    // Se nenhum dado foi lido, libera os recursos, imprime 0 e encerra o programa.
    if (n == 0) {
        free(heights);
        free(widths);
        printf("0\n");
        return 0;
    }

    // Calcula a maior área retangular possível com base nos dados das barras.
    int maxArea = largestRectangleArea(widths, heights, n);
    // Imprime a área encontrada.
    printf("%d\n", maxArea);

    // Libera a memória alocada para os vetores de alturas e larguras.
    free(heights);
    free(widths);
    // Encerra o programa com sucesso.
    return 0;
}
