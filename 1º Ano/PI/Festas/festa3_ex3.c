#include <stdio.h>
#include <assert.h>

#define MAX 10 // Número máximo de cavalos

//  algumas funções auxiliares para a programação defensiva,
//  **optativas** neste exercício

// função que responde à questão: a coordenada i está dentro do tabuleiro?
int valida(int i){return 0<=i && i<8;}

// função que responde à questão: não há já ali um cavalo? 
int ocupado(int cav[2][MAX], int p){
  assert(p<MAX); //facultativo

  int i = p-1;
  for(;i>=0;i--){
    if (cav[0][i] == cav[0][p] && cav[1][i] == cav[1][p]) return 1;
  }
  return 0;
 }

// todos os cavalos registados são válidos e não há repetição
int cav_valido(int cav[2][MAX], int n){
  assert(n<MAX && 0<n); 
  int i = 0;
  for(;i<n;i++){
    if (!valida(cav[0][i]) || !valida(cav[1][i]) || ocupado(cav,i)) return 0;
  }
  return 1;
}


// 1. Função para ler as posições dos cavalos
void ler_cavalos(int cav[2][MAX], int n) {
  assert(n<MAX); //facultativo
  for (int i = 0; i < n; i++) {
    do{
      printf("Digite a posição do cavalo %d (x y): ", i + 1);
      scanf("%d %d", &cav[0][i], &cav[1][i]);
    } while (!valida(cav[0][i]) ||!valida(cav[1][i]) || ocupado(cav, i));
  }
}

// 2. Função que verifica se o cavalo C1 pode atacar o cavalo C2
int ataca(int c1x, int c1y, int c2x, int c2y) {
  //facultativo
  assert(valida(c1x) &&
	 valida(c1y) &&
	 valida(c2x) &&
	 valida(c2y)
	 );
  
  // Verifica todas as possíveis posições de ataque de um cavalo
  // De notar que funciona, mesmo se as verificações saltam fora do tabuleiro
  // Numa versão alternativa, podemos verificar somente as posições válidas
  if ((c1x - 2 == c2x && c1y + 1 == c2y) || 
      (c1x - 2 == c2x && c1y - 1 == c2y) || 
      (c1x + 2 == c2x && c1y + 1 == c2y) || 
      (c1x + 2 == c2x && c1y - 1 == c2y) || 
      (c1x - 1 == c2x && c1y + 2 == c2y) || 
      (c1x - 1 == c2x && c1y - 2 == c2y) || 
      (c1x + 1 == c2x && c1y + 2 == c2y) || 
      (c1x + 1 == c2x && c1y - 2 == c2y)) {
    return 1; // C1 pode atacar C2
  }
  return 0; // C1 não pode atacar C2
} // há uma solução mais elegante e curta! qual?
// Dica: se a difereça dos "x"s for dois, e a diferença dos "y"s for um (ou vice-versa"),
// o que isso nos diz sobre os dois cavalos?? 

// 3. Função para verificar se algum cavalo pode atacar outro
int guerra(int cav[2][MAX], int n) {
  assert(cav_valido(cav, n)); //facultativo (alias... tem um custo - aqui pequeno porque cav é pequeno)

  int i=0,j=0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (ataca(cav[0][i], cav[1][i], cav[0][j], cav[1][j])) {
                return 1; // Encontrado um ataque possível
            }
        }
    }
    return 0; // Nenhum ataque encontrado
}

// Função principal para teste
int main() {
    int cav[2][MAX];
    int n;

    do {
      printf("Digite o número de cavalos (1 a %d): ", MAX);
      scanf("%d", &n);
    } while (n>=MAX || n<1);
    
    ler_cavalos(cav, n);
    
    printf("%s\n",(guerra(cav, n))? "Há pelo menos um ataque possível entre os cavalos.":
                                    "Não há ataques possíveis entre os cavalos.");
    

    return 0;
}
