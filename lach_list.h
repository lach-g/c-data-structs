#ifndef LACH_LIST_H
#define LACH_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef struct ListNode ListNode;

struct ListNode {
  void* data;
  ListNode* next;
};

ListNode* lach_ListNode_create(void* data);
int lach_ListNode_append(ListNode* node, void* data);
void* lach_ListNode_get(ListNode* node, int index);
void lach_ListNode_free(ListNode* head);


//***************************************************************
// 
// Implementations
// 
//***************************************************************


// TODO: Input checks
ListNode* lach_ListNode_create(void* data)
{
  ListNode* node = (ListNode*)calloc(1, sizeof(ListNode));
  if (node == NULL) {
    printf("ListNode* create(void* data) failed to allocate memory\n");
    return NULL;
  }

  // TODO: Should I copy this in like strdup()?
  node->data = data;
  node->next = NULL;
  return node;
}

int lach_ListNode_append(ListNode* node, void* data)
{
  ListNode* new = lach_ListNode_create(data);
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

void* lach_ListNode_get(ListNode* node, int index)
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

void lach_ListNode_free(ListNode* head)
{
  ListNode* curr = head;
  while (curr->next != NULL) {
    ListNode* prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(curr);
}


#endif
