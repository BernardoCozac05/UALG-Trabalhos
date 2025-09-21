#include <stdio.h>  // Para fgets e printf
#include <string.h> // Para strlen

int main(){
char s[10];
scanf("%9s", s);
printf("%zu", strlen(s));

}

////The Answer is It reads a word, until reaching whitespace or 9 characters.             
///For the given input, the string read is "line" (total 4 characters).