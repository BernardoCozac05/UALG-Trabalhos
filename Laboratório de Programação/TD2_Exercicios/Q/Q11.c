#include <stdio.h>

int main(){
    char *str = "abcc";

    *(str + 3) = 'd';
    puts(str);

    return 0;
}


////Answer is "run-time error"