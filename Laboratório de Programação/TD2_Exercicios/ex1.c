#include <stdio.h>
#include <math.h>

int dvalue(int x){
    return 2 * x;
}

int tvalue(int x){
    return 3 * x;
}

int pvalue(int x){
    return 2 * 2;
}

int main(){
    int n, op;

    printf("Digite o tamanho do vetor: ");
    scanf("%d", &n);
    
    int vetor[n];

    printf("Digite os elementos do vetor:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &vetor[i]);
    }

    printf("\nEscolha uma transformacao:\n");
    printf("0 - Dobrar\n");
    printf("1 - Triplicar\n");
    printf("2 - Elevar ao quadrado\n");
    printf("Opcao: ");
    scanf("%d", &op);

    if (op < 0 || op > 2) {
        printf("Opcao invalida!\n");
        return 1;
    }

    int (*vetor_funcoes[])(int) = {dvalue, tvalue, pvalue};

    for (int i = 0; i < n; i++) {
        vetor[i] = vetor_funcoes[op](vetor[i]);
    }

    printf("\nVetor transformado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

}
