#include <stdio.h>  // Para fgets e printf
#include <string.h> // Para strlen

int main(){
char s[10];
scanf("%10c", s);
printf("%zu", strlen(s));

}

////The Answer is It reads exactly 10 characters of any kind (including spaces or terminators), stopping only at EOF.
///s[] will contain "line one\nl" (ending with the first "l" character of line two), with no \0 terminator.