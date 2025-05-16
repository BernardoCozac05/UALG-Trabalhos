#include <stdio.h>

int main(int argc, char *argv[]) {
    for(char *p; (p = *++argv);) {
        for (; *p; ++p)
        putchar(*p);
    }
}



////Answer is "onetwothree"
