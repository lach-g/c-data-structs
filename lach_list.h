#ifndef LACH_LIST_H
#define LACH_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef struct LListNode LListNode;

struct LListNode {
  void* data;
  LListNode* next;
};

 
// TODO: Input checks
LListNode* lach_LListNode_create(void* data)
{
  LListNode* node = (LListNode*)calloc(1, sizeof(LListNode));
  if (node == NULL) {
    printf("ListNode* create(void* data) failed to allocate memory\n");
    return NULL;
  }

  // TODO: Should I copy this in like strdup()?
  node->data = data;
  node->next = NULL;
  return node;
}

int lach_LListNode_append(LListNode* node, void* data)
{
  LListNode* new = lach_LListNode_create(data);
  if (new == NULL) {
    printf("int append(ListNode* node, void* data) failed to create a ListNode\n");
    return 1;
  }
  
  while (node->next != NULL) {
    node = node->next;
  }
  
  node->next = new;
  return 0;
}

void* lach_LListNode_get(LListNode* node, int index)
{
  if (index < 0) {
    printf("Index %d must be 0 or greater.\n", index);
    return NULL;
  }
  
  int i = 0;
  while (i < index && node->next != NULL) {
    node = node->next;
    i++;
  }

  if (i == index) {
    return node->data;
  }
  
  printf("Could not find ListNode at index %d. List ended at index %d", index, i);
  return NULL;
}

void lach_LListNode_free(LListNode* head)
{
  LListNode* curr = head;
  while (curr->next != NULL) {
    LListNode* prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(curr);
}


#endif
