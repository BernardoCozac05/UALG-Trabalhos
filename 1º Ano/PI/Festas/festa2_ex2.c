#include <stdio.h>
#include <assert.h>
typedef long long int lli;

lli pellrt(int n, lli p2, lli p1) {return (!n)?p2:(n==1)?p1:pellrt(n - 1, p1, 2 * p1 + p2);}

lli pell(int n) {return pellrt(n, (lli) 0, (lli) 1);}

int main() {
    int n=0;
    if ((scanf(" %d", &n) != 1) && n<0){printf("NO\n"); return 0;}
    printf("%lld\n", pell(n));
    return 0;
}
