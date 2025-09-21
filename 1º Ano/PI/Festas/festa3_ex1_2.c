#include<stdio.h>
#include<limits.h>
#include<assert.h>

long long int direct(int n){
assert(n>=0);
 return ((((long long int) n) * (n+1LL) * (2*n + 1LL))) / 6LL; 
}

// Alínea 4 -  O problema do calculo da componente esquerda está no cálculo de
// 1^2 + 2^2 + .. + n^2 por cada n.
// Informalmentee, para um n em particular, temos n somas.
// Se calculamos para 0, 1, e, 3  ... n, teremos a volta de n^2 somas
// por fazer (o número exacto é (n*(n+1))/2).
// Com n bastante grande, isso é muito. Daí a demora!
// Ora vejamos: 
// para n+1,. calcula-se 1^2 + 2^2 + .. + n^2 + (n+1)^2, ou seja,
// repetimos um cálculo anterior ao qual juntamos (n+1)^2.
// É muito menos trabalho! desde que me lembre do calculo anterior.
// A nova versão da função tira proveito desta ideia.
//


// Alíneas 5 e 6
long long int sum_square_bis(int n, long long int last){
  assert(n>=0); // programação defensiva, opcional
  return last + ((long long int)n)*n; // o cast está aqui para obrigar a que a multiplicação 
                                      // seja tipo long long int e evitar overflow
}


int discorda(void){
  int n=0;
  long long int left=0,right=0;
  do{
    left=sum_square_bis(n, left);
    right=direct(n);
    n++;
  }
  while(n>=0 && left==right);
  return (left==right)?-1:n-1; 
}

int main(){
  int n = discorda ();
  if (n>=0) printf("Discorda em %d\n",n);
  else printf("Não há discordância!\n");
  return 0;
}



