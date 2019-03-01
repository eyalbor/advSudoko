#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "bool.h"
#include "list.h"

void list_new(list *list, int elementSize, freeFunction freeFn)
{
  assert(elementSize > 0);
  list->logicalLength = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->freeFn = freeFn;
  list->currentElement = NULL;
}

void list_destroy(list *list)
{
  listNode *current;
  while(list->head != NULL) {
    current = list->head;
    list->head = current->next;

    if(list->freeFn) {
      list->freeFn(current->data);
    }
    free(current);
  }
}

void list_deleteAfter(list *list){
  listNode *current, *next;
  if(list->currentElement != NULL){
	  current = list->currentElement->next;
	  next = current;
	  while(current != NULL) {
		if(list->freeFn) {
		  list->freeFn(current->data);
		}
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	  }
  }
}

void list_appendAfter(list *list, void *elementToAdd){
	if(list->elementSize == 0){
		list_prepend(list, elementToAdd);
	}else{
		listNode *node = malloc(sizeof(listNode));
		 node->data = elementToAdd;

	    /* 4. Make next of new node as next of prev_node */
		node->next = list->currentElement->next;

	    /* 5. Make the next of prev_node as new_node */
		list->currentElement->next = node;

	    /* 6. Make prev_node as previous of new_node */
	    node->prev = list->currentElement;

	    /* 7. Change previous of new_node's next node */
	    if (node->next != NULL)
	        node->next->prev = node;

	    list->currentElement = node;

	}
	list->elementSize++;

}

listNode* list_getCurrentElement(list* list){
	return list->currentElement;
}

/**
 * adds a node to the head of the list
 */
void list_prepend(list *list, void *element)
{
  listNode *node = malloc(sizeof(listNode));
  node->data = element;

  node->next = list->head;

  node->prev = NULL;
  if(list->head != NULL){
	  list->head->prev = node;
  }

  list->head = node;
  /* first node? */
  if(!list->tail) {
    list->tail = list->head;
  }
  list->currentElement = list->head;
  list->logicalLength++;
}

/**
 * adds a node to the tail of the list
 */
void list_append(list *list, void *element)
{
  listNode *node = malloc(sizeof(listNode));
  node->next = NULL;
  node->prev = NULL;

  node->data = element;

  if(list->logicalLength == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  list->currentElement = list->tail;
  list->logicalLength++;
}

void list_for_each(list *list, listIterator iterator)
{
  assert(iterator != NULL);

  listNode *node = list->head;
  bool result = TRUE;
  while(node != NULL && result) {
    result = iterator(node->data);
    node = node->next;
  }
}

/**
 * remove the head from the list
 */
void list_removeHead(list *list){
	assert(list->head != NULL);
	listNode *node = list->head;
	list->head = node->next;
	list->head->prev = NULL;
	list->logicalLength--;
	list->currentElement = list->head;
	list->freeFn(node->data);
	free(node);
}

listNode* list_head(list *list)
{
  return list->head;
}

listNode* list_tail(list *list)
{
  return list->tail;
}

int list_size(list *list)
{
  return list->logicalLength;
}

/* This function prints contents of linked list starting from the given node */
void printList(list* list , printFunc printFunc)
{
    listNode* node;
    printf("\nTraversal in forward direction \n");
    if(list==NULL){
    	return;
    }
    node = list->head;
    while (node != NULL) {
    	printFunc(node->data);
        node = node->next;
    }
}

void list_undoCurrentElement(list* list){
	if(list->currentElement!=NULL){
		list->currentElement = list->currentElement->prev;
	}
}

void list_redoCurrentElement(list* list){
	if(list->currentElement!=NULL){
		list->currentElement = list->currentElement->next;
	}
}
