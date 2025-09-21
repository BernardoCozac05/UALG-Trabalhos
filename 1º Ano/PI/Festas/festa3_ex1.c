#include<stdio.h>
#include<limits.h>
#include<assert.h>

// alíneas 1,2 e 3

int direct(int n){
assert(n>=0);
return (n * (n+1) * (2*n +1)) / 6; 
}


int sum_square(int n){
assert(n>=0);
 int i;
 int sum=0;
 for(i=0;i<=n;i++){
   sum+=i*i;
 }
 return sum;
}


int discorda(void){
  int n=0;
  int left=0,right=0;
  do{
    left=sum_square(n);
    right=direct(n);
    n++;
  }
  while(n>=0 && left==right);
  // se ambos valores permanecem iguais fora do ciclo,
  // então não há discordância, então devolvemos -1,
  // senão devolvemos o valor da discordância, n-1,
  // pois, fizemos n++ antes de sair.
  return (left==right)?-1:n-1; 
}

int main(){
  int n = discorda ();
  if (n>=0) printf("Discorda em %d\n",n);
  else printf("Não há discordância!\n");
  return 0;
}



