#ifndef LACH_LIST_H
#define LACH_LIST_H

#include <stdlib.h>
#include <stdio.h>

#include "lach_log.h"

typedef struct LListNode LListNode;

struct LListNode {
  void*         data;   // The pointer to the data stored by the list node.
  LListNode*    next;   // The next node in the list.
  LListNode*    prev;   // The previous node in the list.
};

typedef struct {
  size_t        count;  // The number of list nodes in the list.
  size_t        size;   // The size of data type stored by each list node.
  LListNode*    head;   // The head of the list.
  LListNode*    tail;   // The tail of the list.
} LList;


/*
 * Create a doubly-linked list.
 *
 * @param size The size of the data type to be stored by each list node.
 * @return LList* A pointer to the newly allocated list.
 */
LList* lach_LList_create(size_t size)
{
  LList* list = (LList*)calloc(1, sizeof(LList));
  if (list == NULL) {
    lach_log(LLOG_ERROR, "Failed to allocate memory for LList.");
    return NULL;
  }

  list->size = size;

  return list;
}

/*
 * Create a list node with data.
 *
 * @param data A pointer to data to allocate and assign to the new list node.
 * @param size The size of the data type to be stored by the list node.
 * @return LListNode* A pointer to the newly allocated list node.
 */
LListNode* lach_LListNode_create(void* data, size_t size)
{
  if (data == NULL) {
    lach_log(LLOG_ERROR, "Data is NULL.");
    return NULL;
  }
  
  LListNode* node = (LListNode*)calloc(1, sizeof(LListNode));
  if (node == NULL) {
    lach_log(LLOG_ERROR, "Failed to allocate memory for ListNode.");
    return NULL;
  }

  node->data = data;
  memcpy(node->data, data, size);

  return node;
}

/*
 * Add to tail of doubly-linked list.
 */
int lach_LList_append(LList* list, void* data)
{
  if (list == NULL) {
    lach_log(LLOG_ERROR, "List is NULL.");
    // TODO: LList error codes
    return 1;
  }
  
  if (data == NULL) {
    lach_log(LLOG_ERROR, "Data is NULL.");
    return 1;
  }
  
  LListNode* new_node = lach_LListNode_create(data, list->size);
  if (new_node == NULL) {
    lach_log(LLOG_ERROR, "Failed to append new ListNode.");
    return 1;
  }

  if (list->count == 0) {
      list->head = new_node;
      list->tail = new_node;
  } else {
      new_node->next = list->tail;
      list->tail = new_node;
  }
  list->count++;

  return 0;
}

/*
 * Add to head of doubly-linked list.
 */
int lach_LList_prepend(LList* list, void* data)
{
    if (list == NULL) {
        lach_log(LLOG_ERROR, "List is NULL.");
        // TODO: LList error codes
        return 1;
    }

    if (data == NULL) {
        lach_log(LLOG_ERROR, "Data is NULL.");
        return 1;
    }

    LListNode* new_node = lach_LListNode_create(data, list->size);
    if (new_node == NULL) {
        lach_log(LLOG_ERROR, "Failed to append new ListNode.");
        return 1;
    }

    if (list->count == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->prev = list->head;
        list->head = new_node;
    }
    list->count++;

    return 0;
}

void* lach_LList_at(LList* list, int index)
{
  if (node == NULL) {
    lach_log(LLOG_ERROR, "Node is NULL.");
    return NULL;
  }
  
  if (index < 0) {
    lach_log(LLOG_ERROR, "Index %d must be 0 or greater.", index);
    return NULL;
  } else if (index >= list->count) {
      lach_log(LLOG_ERROR, "Index %d must be less than the list count %zu.", index, list->count);
      return NULL;
  }

  // TODO: Is there a better iterator way than this?
  int i = 0;
  for (LListNode* node = list->head; node != NULL; node = node->next) {
      (i != index) ? i++ : return node;
  }

  lach_log(LLOG_ERROR, "Could not find ListNode at index %d. List ended at index %d with count.", index, i, list->count);

  return NULL;
}

int lach_LList_insert(LList* list, int index)
{
    if (node == NULL) {
        lach_log(LLOG_ERROR, "Node is NULL.");
        return NULL;
    }

    if (index < 0) {
        lach_log(LLOG_ERROR, "Index %d must be 0 or greater.", index);
        return NULL;
    } else if (index >= list->count) {
        lach_log(LLOG_ERROR, "Index %d must be less than the list count %zu.", index, list->count);
        return NULL;
    }

    LListNode* new_node = lach_LListNode_create(data, list->size);
    if (new_node == NULL) {
        lach_log(LLOG_ERROR, "Failed to append new ListNode.");
        return 1;
    }

    if (list->count == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        // TODO: Is there a better iterator way than this?
        int i = 0;
        for (LListNode* node = list->head; node != NULL; node = node->next) {
            if (i == index) {
                if (node->prev) {
                    node->prev->next = new_node;
                    new_node->prev = node->prev;
                } else {
                    // TODO: was trying to be clever with taking care of tail and head edge cases with middle
                    // but if 0 index or count - 1 index specified im not sure if this will work... I'm tired w
                    list->tail = new_node;
                }

                if (node->next)
            }
        }

    }


    lach_log(LLOG_ERROR, "Could not find index %d. List ended at index %d with count.", index, i, list->count);

    return 1;
}

int lach_LListNode_free(LListNode* head)
{
  if (head == NULL) {
    printf("Head is NULL.\n");
    return 1;
  }
  
  LListNode* curr = head;
  while (curr->next != NULL) {
    LListNode* prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(curr);

  return 0;
}


#endif
