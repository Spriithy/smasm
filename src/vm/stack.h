#ifndef STACK_H
#define STACK_H

#include "memory.h"

#define STACK_SIZE_INITIAL 1 << 8
#define STACK_SIZE_GROW 1 << 6

typedef struct {
  int size;
  int capacity;
  word *data;
} stack;

stack *new_stack(int size);

word stack_pop(stack *s);
int stack_push(stack *s, word w);

#endif  // STACK_H
