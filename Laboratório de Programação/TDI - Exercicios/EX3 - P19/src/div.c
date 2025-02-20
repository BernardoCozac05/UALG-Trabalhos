#include "div.h"
#include <errno.h>

int safe_div(int a, int b) {  // Changed function name to safe_div
    if (b == 0) {
        errno = EDOM; // Define domain error
        return 0;
    }
    return a / b;
}
