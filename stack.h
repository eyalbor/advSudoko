#ifndef __STACK_H
#define __STACK_H

/* https://pseudomuto.com/2013/06/implementing-a-generic-stack-in-c/ */

typedef struct {
  list *list;
} stack;

void stack_new(stack *s, int elementSize, freeFunction freeFn);
void stack_destroy(stack *s);
void stack_push(stack *s, void *element);
void stack_pop(stack *s, void *element);
void* stack_peek(stack *s);
int stack_size(stack *s);

#endif
