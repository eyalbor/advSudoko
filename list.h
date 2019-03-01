/**
 * generic list we take from and add some functionality
 * https://pseudomuto.com/2013/05/implementing-a-generic-linked-list-in-c/
 */

#ifndef __LIST_H
#define __LIST_H

/* a common function used to free malloc'd objects */
typedef void (*freeFunction)(void *);

/* a common function used to free malloc'd objects */
typedef void (*printFunc)(void *);

typedef bool (*listIterator)(void *);

typedef struct _listNode {
  void *data;
  struct _listNode *next;
  struct _listNode *prev;
} listNode;

typedef struct {
  int logicalLength;
  int elementSize;
  listNode *head;
  listNode *tail;
  listNode * currentElement;
  freeFunction freeFn;
} list;


/**
 * initializes a linked list to store elements of elementSize and to call freeFunction
 *  for each element when destroying a list
 */
void list_new(list *list, int elementSize, freeFunction freeFn);

/**
 * frees dynamically allocated nodes and optionally calls freeFunction with each node’s data pointer
 */
void list_destroy(list *list);

/**
 * adds a node to the head of the list
 */
void list_prepend(list *list, void *element);

/**
 * adds a node to the tail of the list
 */
void list_append(list *list, void *element);

/**
 * delete all Nodes after the currentElement Node
 */
void list_deleteAfter(list *list);

/**
 * add elementToAdd Note after currentElement Node
 */
void list_appendAfter(list *list, void *elementToAdd);

/**
 * return the position of where the user add the last Node to list
 */
void* getCurrentElement(list* list);

/**
 * returns the number of items in the list
 */
int list_size(list *list);

/*
 * calles the supplied iterator function with the data element of each node (iterates over the list)
 */
void list_for_each(list *list, listIterator iterator);

/**
 * returns the head of the list (optionally removing it at the same time)
 */
void list_head(list *list, void *element, bool removeFromList);

/**
 * returns the tail of the list
 */
void list_tail(list *list, void *element);

/**
 * print the list
 */
void printList(list* node , printFunc printFunc);

#endif
