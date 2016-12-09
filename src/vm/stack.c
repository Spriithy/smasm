#include "stack.h"
#include <stdlib.h>
#include "log.h"

/* Creates and allocates a new instance of a stack object for the VM. */
stack *new_stack(int size) {
  stack *s = calloc(1, sizeof(stack));
  s->data = malloc(size * sizeof(word));
  s->capacity = size;
  s->size = 0;
  return s;
}

/* grows a stack by the given amount of words */
static int stack_grow(stack *s, int by) {
  s->capacity += by;
  s->data = realloc(s->data, s->capacity * sizeof(word));
  return s->data != NULL;  // return whether it was successful or not
}

/* pops a word off of the stack and returns it.
 * Stops the execution if stack is empty
 */
word stack_pop(stack *s) {
  if (s->size > 0) return s->data[--s->size];
  VM_ERROR("%s", "stack underflow");
}

/* pushes a word on the stack.
 * Stops the execution if stack is full and couldn't be grown
 */
int stack_push(stack *s, word w) {
  if (s->size < s->capacity)
    //  if (stack_grow(s, STACK_SIZE_GROW)) VM_ERROR("%s", "stack overflow");
    s->data[s->size++] = w;
  return 0;
}
