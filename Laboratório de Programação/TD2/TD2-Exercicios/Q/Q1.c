#include <stdio.h>

int main(void){

    int x, y;
    int i = 3;
    x = y = 0;
    while (i >= 0){
        x+=1;
        i--;
    }
    do {y += 1; i++;}
    while (i <= 3);
    printf("%d\n", (x-y));

}

/////Answer is "-1"