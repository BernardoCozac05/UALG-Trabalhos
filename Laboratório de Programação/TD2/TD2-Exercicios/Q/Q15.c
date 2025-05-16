#include <stdio.h>  // Para fgets e printf
#include <string.h> // Para strlen

int main(){
char s[10];
fgets (s, 10, stdin);
printf("%zu", strlen(s));

}

////The Answer is It reads a line, until reaching \n, EOF, or the limit (10-1 chars). 
////For the given input, the string read is "line one\n" (total 9 characters).