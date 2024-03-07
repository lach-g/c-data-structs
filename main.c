
#include <stdio.h>
#include <assert.h>

#include "lach_list.h"
#include "lach_hashtable.h"
#include "lach_vector.h"

typedef struct {
  int id;
  char data_type;
  long value;
} Tag;

void print_tag(Tag* tag)
{
  printf("Tag(id=%d, data_type=%c, value=%ld)\n", tag->id, tag->data_type, tag->value);
}

void test_ListNode(void)
{
  Tag tag1 = { .id = 1, .data_type = 'I', .value = 42 };
  Tag tag2 = { .id = 2, .data_type = 'J', .value = 69 };
  Tag tag3 = { .id = 3, .data_type = 'K', .value = 420 };

  ListNode* head = lach_ListNode_create((void*)&tag1);
  lach_ListNode_append(head, (void*)&tag2);
  lach_ListNode_append(head, (void*)&tag3);

  Tag* maybe_1 = (Tag*)lach_ListNode_get(head, 0);
  Tag* maybe_2 = (Tag*)lach_ListNode_get(head, 1);
  Tag* maybe_3 = (Tag*)lach_ListNode_get(head, 2);

  // Test tag 1
  assert(maybe_1->id == tag1.id);
  assert(maybe_1->data_type == tag1.data_type);
  assert(maybe_1->value == tag1.value);

  // Test tag 2
  assert(maybe_2->id == tag2.id);
  assert(maybe_2->data_type == tag2.data_type);
  assert(maybe_2->value == tag2.value);

  // Test tag 3
  assert(maybe_3->id == tag3.id);
  assert(maybe_3->data_type == tag3.data_type);
  assert(maybe_3->value == tag3.value);
  
  
  lach_ListNode_free(head);
}

void test_HashTable(void)
{
  char tag1_name[] = "tag1";
  char tag2_name[] = "tag2";
  char tag3_name[] = "tag3";
  
  Tag tag1 = { .id = 1, .data_type = 'I', .value = 42 };
  Tag tag2 = { .id = 2, .data_type = 'J', .value = 69 };
  Tag tag3 = { .id = 3, .data_type = 'K', .value = 420 };

  HashTable* table = lach_HashTable_create(128);
  lach_HashTable_insert(table, tag1_name, (void*)&tag1);
  lach_HashTable_insert(table, tag2_name, (void*)&tag2);
  lach_HashTable_insert(table, tag3_name, (void*)&tag3);

  Tag* maybe_1 = (Tag*)lach_HashTable_get(table, tag1_name);
  Tag* maybe_2 = (Tag*)lach_HashTable_get(table, tag2_name);
  Tag* maybe_3 = (Tag*)lach_HashTable_get(table, tag3_name);

  // Test tag 1
  assert(maybe_1->id == tag1.id);
  assert(maybe_1->data_type == tag1.data_type);
  assert(maybe_1->value == tag1.value);

  // Test tag 2
  assert(maybe_2->id == tag2.id);
  assert(maybe_2->data_type == tag2.data_type);
  assert(maybe_2->value == tag2.value);

  // Test tag 3
  assert(maybe_3->id == tag3.id);
  assert(maybe_3->data_type == tag3.data_type);
  assert(maybe_3->value == tag3.value);
}

void test_Vector(void)
{
  Tag tag1 = { .id = 1, .data_type = 'I', .value = 42 };
  Tag tag2 = { .id = 2, .data_type = 'J', .value = 69 };
  Tag tag3 = { .id = 3, .data_type = 'K', .value = 420 };

  Vector* vec = lach_Vector_create(0);
  if (vec == NULL) {
    printf("Failed creating vector.\n");
    exit(1);
  }
  
  lach_Vector_append(vec, &tag1);
  lach_Vector_append(vec, &tag2);
  lach_Vector_append(vec, &tag3);
  
  Tag* maybe_1 = (Tag*)lach_Vector_get(vec, 0);
  Tag* maybe_2 = (Tag*)lach_Vector_get(vec, 1);
  Tag* maybe_3 = (Tag*)lach_Vector_get(vec, 2);

  // Test tag 1
  assert(maybe_1->id == tag1.id);
  assert(maybe_1->data_type == tag1.data_type);
  assert(maybe_1->value == tag1.value);

  // Test tag 2
  assert(maybe_2->id == tag2.id);
  assert(maybe_2->data_type == tag2.data_type);
  assert(maybe_2->value == tag2.value);

  // Test tag 3
  assert(maybe_3->id == tag3.id);
  assert(maybe_3->data_type == tag3.data_type);
  assert(maybe_3->value == tag3.value);
}



int main(void)
{
  test_HashTable();
  printf("HashTable test completed.\n");
  
  test_Vector();
  printf("Vector test completed.\n");
  
  test_ListNode();
  printf("ListNode test completed.\n");
  
  printf("Tests ran successfully.\n");

  return 0;
}
