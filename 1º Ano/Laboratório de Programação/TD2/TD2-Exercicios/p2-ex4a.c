#include <stdio.h>
#include <stdlib.h>
//ex4, pag 36

int* retorna_ponteiro(){
    int p = 33;
    return &p;
}

int main(void){

    int* p = retorna_ponteiro();
    printf("endereço de p: %p\n", p);

    printf("valor de p: %i\n", (*p));

    return 0;

}
