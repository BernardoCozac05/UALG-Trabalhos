#include <stdio.h>
#include <assert.h>

int max(int a, int b){return (a<b)?b:a;}
int abs(int a){return (a<0)?-a:a;}

int max_digit(int n, int maxd) {
  assert(n>=0);
  if (n < 10) return max(n,maxd);
  int lastd = n % 10;
  int remd = n / 10;
  return max_digit(remd, max(maxd,lastd));
}

int main() {
  int n;
  if (scanf(" %d", &n)!=1){printf("NO\n");return 0;}
  printf("%d\n", max_digit(abs(n),0));
  return 0;
}
