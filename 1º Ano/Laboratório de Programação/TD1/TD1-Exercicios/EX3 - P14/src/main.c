#include <stdio.h>
#include "div.h"

int main() {
  int a = 10, b = 2;
  int result = div(a, b); 
  
#ifdef DEBUG_ON
  
  printf("[DEBUG] chamando div(%d,%d)\n",a,b);    

#endif
  
  printf("Resultado: %d / %d = %d\n", a, b, result);

  // Testando div
  b = 0;
  result = div(a, b);
  printf("Resultado: %d / %d = %d\n", a, b, result);
  
  return 0;
}
