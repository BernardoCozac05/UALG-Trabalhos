#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int largura;
    int altura;
} caixa;

int main() {
    caixa *caixas = NULL;
    int capacidade = 2; // Tamanho inicial do Array
    int size = 0; // Numero de elementos lidos
    int maior_area = 0; // Area da maior caixa
    caixas = (caixa *) malloc(capacidade * sizeof(caixa)); // Alocar memória para o array

    if(!caixas) {
        printf("Erro ao alocar memória\n"); // Memória mal alocada.
        return 1;
    }

    printf("Insira pares de largura e altura: \n");

    while(scanf("%d %d", &caixas[size].largura, &caixas[size].altura) == 2) { // Ler os dados LARGURA / ALTURA.
        size++;

         // Calcular a área da caixa
         int area = caixas[size - 1].largura * caixas[size - 1].altura;

         // Verificar se a área atual é maior que a maior área encontrada
         if (area > maior_area) {
             maior_area = area;
         }


        if (size >= capacidade) {
                capacidade *=2;
                caixas = (caixa *) realloc(caixas, capacidade * sizeof(caixa));
                if (!caixas){
                    printf("Erro ao alocar memória\n");
                    return 1;
                }
                
        }
        

    }
    for (int i = 0; i < size; i++) {

        

        printf("Caixa %d -> Largura: %d, Altura: %d\n", i + 1, caixas[i].largura, caixas[i].altura);
        
    }   
    printf("%d\n",maior_area);

    free(caixas); // Libertar a memória
    return 0;
}