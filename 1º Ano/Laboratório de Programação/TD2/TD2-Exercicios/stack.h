#ifndef STACH_H
#define STACK_H

#include <stdbool.h>

#define STACK_SIZE 1000

typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

void init(Stack *s);
void push(Stack *s, int value);
int pop(Stack *s);
int top(Stack *s);
bool is_empty(Stack *s);

#endif
