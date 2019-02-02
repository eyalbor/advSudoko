#ifndef __STACK_H
#define __STACK_H

// from previous post on linked-list implementation
// in C
#include "list.h"

typedef struct {
  list *list;
} stack;

void stack_new(stack *s, int elementSize, freeFunction freeFn);
void stack_destroy(stack *s);
void stack_push(stack *s, void *element);
void stack_pop(stack *s, void *element);
void stack_peek(stack *s, void *element);
int stack_size(stack *s);

#endif
