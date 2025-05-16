#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef long long int lli;

int digits(int n, int d) {return (!n)?d:digits(n / 10, d+1);}
lli power_sum(int n, int d) {return (!n)?0:((lli)pow(n%10, d)) + power_sum(n / 10, d);}

int is_narcissistic(int n) {return (n < 0)?0:(n == power_sum(n,digits(n,0)));}

int main() {
    int n;
    if ((scanf("%d", &n) != 1) && n<0) {printf("NO\n");return 0;}
    printf("%s\n",(is_narcissistic(n))?"YES":"NO");
    return 0;
}
