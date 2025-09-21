#include<stdio.h>
#include <float.h> 
#include<assert.h>

#define MAXS 1000
#define MAXM 500

// alinea 1:
//float mat[MAXS][MAXM]; 

void read_data(float mat[MAXS][MAXM], int s, int m) {
  int i = 0, j = 0;
  // Lê os dados de temperatura para cada sensor e cada medição
  for (; i < s; i++) {
    for (j = 0; j < m; j++) {
      scanf("%f", &mat[i][j]);  // Lê a temperatura para o sensor i e medição j
    }
  }
}


float maior(float mat[MAXS][MAXM], int s, int m) {
  float maior_temp = mat[0][0];
  int i = 0, j = 0;
  for (; i < s; i++) {
    for (j = 0; j < m; j++) {
      if (mat[i][j] > maior_temp) {
	// Atualiza se encontrar uma temperatura maior
	maior_temp = mat[i][j];  
      }
    }
  }
  
  return maior_temp; 
}


void media_por_sensor(float mat[MAXS][MAXM], int s, int m, float medias[MAXS]) {
  // Calcula a média de temperaturas por sensor
  int i = 0, j = 0, soma = 0;
  for (; i < s; i++) {
    soma = 0.0;
    for (j = 0; j < m; j++) {
      soma += mat[i][j];  // Soma todas as temperaturas do sensor i
    }
    medias[i] = soma / m;  // Calcula a média e armazena no vetor medias
  }
}


int main() {  
  // Declara a matriz para armazenar as temperaturas
  float mat[MAXS][MAXM]; 
  
  // Vetor para armazenar a média das temperaturas por sensor
  float medias[MAXS];
  int s, m;
  
  // Lê o número de sensores e o número de medições
  do{
    printf("Digite o número de sensores (s): ");
    scanf("%d", &s);
  } while(s>MAXS || s<1);
  
  do{
    printf("Digite o número de medições por sensor (m): ");
    scanf("%d", &m);
  } while (m>MAXM || m<1);
  
  // Lê os dados de temperatura
  read_data(mat, s, m);
  
  // Encontra a maior temperatura
  float maior_temp = maior(mat, s, m);
  printf("A maior temperatura registrada foi: %.2f\n", maior_temp);
  
  // Calcula a média das temperaturas por sensor
  media_por_sensor(mat, s, m, medias);
  
  // Exibe as médias de cada sensor
  printf("\nMédia das temperaturas por sensor:\n");
  for (int i = 0; i < s; i++) {
    printf("Sensor %d: %.2f\n", i + 1, medias[i]);
  }
  
  return 0;
}

