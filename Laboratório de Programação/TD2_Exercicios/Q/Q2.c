#include <stdio.h>

int main(void) {
    int x, y;

    int ary[4] = {11, 22, 33, 44};
    int *ptr;

    ptr = ary;
    ptr++;

    printf("%d %p\n", *ptr, ptr);

}

////Answer is "22 0x7ffc3ae795e4"