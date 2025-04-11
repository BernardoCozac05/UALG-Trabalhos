#include <stdio.h>     // For printf
#include <stdlib.h>    // For malloc, free, exit
#include <string.h>    // For strlen, strcpy, strtok

int main() {
    char *t;
    const char *str = "item1 item2 item3 item4 item5";

    char *tmp = (char *)malloc(strlen(str) + 1);
    if (tmp == NULL) {
            exit(1);
    }
    strcpy(tmp, str);
    t = strtok(tmp, " ");
    printf("%s,", t);

    while (t = strtok(0, " ")) {
            printf("%s,", t);
    }

    free(tmp);
    tmp = NULL;

}

///////Answer is "item1,item2,item3,item4,item5,", with the libraries, without it just dont compile