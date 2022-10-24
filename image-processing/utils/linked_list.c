#include "linked_list.h"

LinkedList *lkAppend(LinkedList *list, void *value)
{
  LinkedList *new = malloc(sizeof(LinkedList));
  if (new == NULL)
  {
    errx(1, "malloc");
  }
  new->value = value;
  new->next  = NULL;
  new->prev  = list;
  list->next = new;
  return new;
}

void lkRemove(LinkedList *list)
{
  if (list->prev != NULL)
  {
    list->prev->next = list->next;
  }
  if (list->next != NULL)
  {
    list->next->prev = list->prev;
  }
  free(list);
}

LinkedList *lkGetIndex(LinkedList *list, int index)
{
  for (int i = 0; i < index; i++)
  {
    list = list->next;
  }
  return list;
}

void lkFreeLinkedList(LinkedList *list)
{
  while (list->next != NULL)
  {
    list = list->next;
    free(list->prev);
  }
  free(list);
}

size_t lkLen(LinkedList *list)
{
  size_t len = 0;
  while (list->next != NULL)
  {
    list = list->next;
    len++;
  }
  return len;
}