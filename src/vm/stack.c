#include "stack.h"
#include <stdlib.h>
#include "log.h"

stack *new_stack(int size) {
  stack *s = calloc(1, sizeof(stack));
  s->data = malloc(size * sizeof(word));
  s->capacity = size;
  s->size = 0;
  return s;
}

int stack_grow(stack *s, int by) {
  s->capacity += by;
  s->data = realloc(s->data, s->capacity * sizeof(word));
  return s->data != NULL;
}

word stack_pop(stack *s) {
  if (s->size > 0) return s->data[--s->size];
  VM_ERROR("%s", "stack underflow");
}

void stack_push(stack *s, word w) {
  if (s->size == s->capacity)
    if (stack_grow(s, STACK_SIZE_GROW)) VM_ERROR("%s", "stack overflow");
  s->data[s->size++] = w;
}
