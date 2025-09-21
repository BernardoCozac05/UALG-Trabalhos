#include <stdio.h>

int main() {
    char arr[] = "abcc";

    *(arr + 3) = 'd';
    puts(arr);

    return 0;
}


///////Answer is "abcd"