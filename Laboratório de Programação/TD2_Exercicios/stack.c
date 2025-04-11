#include <stdio.h>
#include "stack.h"
void init(Stack *s) {
s->top = -1;
}
void push(Stack *s, int value) {
if (s->top == STACK_SIZE - 1) {
printf("Error: Stack Overflow!\n");
return;
}
s->data[++(s->top)] = value;
printf("Pushed: %d\n", value);
}
int pop(Stack *s) {
if (is_empty(s)) {
printf("Error: Stack Underflow!\n");
return -1; // valor para indcar um erro
}
int value = s->data[(s->top)--];
printf("Popped: %d\n", value);
return value;
}

int top(Stack *s) {
    if (is_empty(s)) {
    printf("Error: Stack is empty!\n");
    return -1;
    }
    printf("Top: %d\n", s->data[s->top]);
    return s->data[s->top];
    }
    bool is_empty(Stack *s) {
    return s->top == -1;
    }