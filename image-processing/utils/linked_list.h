#pragma once
#include <err.h>
#include "stdlib.h"

typedef struct LinkedList
{
  void              *value;
  struct LinkedList *next;
  struct LinkedList *prev;
} LinkedList;

/**
 * @brief Append an element to the list.
 *
 * @param list
 * @param value
 * @return LinkedList*
 */
LinkedList *lkAppend(LinkedList *list, void *value);

/**
 * @brief Remove an element from the list.
 *
 * @param list
 */
void lkRemove(LinkedList *list);

/**
 * @brief Get the element at index from the list.
 *
 * @param list
 * @param index
 * @return LinkedList*
 */
LinkedList *lkGetIndex(LinkedList *list, int index);

/**
 * @brief Free the list.
 *
 * @param list
 */
void lkFreeLinkedList(LinkedList *list);

/**
 * @brief Get the length of the list.
 *
 * @param list
 * @return size_t
 */
size_t lkLen(LinkedList *list);
