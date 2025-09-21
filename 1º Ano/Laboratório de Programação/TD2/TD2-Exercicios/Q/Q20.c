#include <stdlib.h>
#include <stdio.h>

int* retorna_ponteiro(){
    int p = 33;
    return &p;
}

int main(void){
    int* p = retorna_ponteiro();
    printf("valor de p: %i\n", (*p));
    return 0;
}

///NO, prints 33