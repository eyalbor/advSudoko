#include <stdlib.h>
#include <assert.h>
#include "bool.h"
#include "list.h"
#include "stack.h"

void stack_new(stack *s, int elementSize, freeFunction freeFn)
{
  s->list = malloc(sizeof(list));
  /* make sure the malloc call didn't fail...*/
  assert(s->list != NULL);

  list_new(s->list, elementSize, freeFn);
}

void stack_destroy(stack *s)
{
  list_destroy(s->list);
  free(s->list);
}

void stack_push(stack *s, void *element)
{
  list_prepend(s->list, element);
}

void stack_pop(stack *s)
{

	/* don't pop an empty stack! */
	assert(stack_size(s) > 0);
	/*node = list_head(s->list);*/
	/*element = node->data;*/
	list_removeHead(s->list);
}

void* stack_peek(stack *s)
{
	listNode* node;
	/* don't pop an empty stack! */
	assert(stack_size(s) > 0);
	node = list_head(s->list);
	return  node->data;

}

int stack_size(stack *s)
{
  return list_size(s->list);
}
