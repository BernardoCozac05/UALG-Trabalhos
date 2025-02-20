
#include "counter.h"

static unsigned int counter = 0;

void incr(){
  counter = counter + 1;
}

void reset(){
  counter = 0;
}

unsigned int get() {
  return counter;
}
