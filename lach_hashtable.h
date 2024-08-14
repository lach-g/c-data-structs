#ifndef LACH_HASHTABLE_H
#define LACH_HASHTABLE_H


#include <stdlib.h>
#include <stdio.h>

#define LACH_HASHTABLE_DEFAULT_CAPACITY 255

typedef struct {
  char* key;
  void* value;
} LHashNode;

// TODO: Use a vector?
typedef struct {
  size_t capacity;
  size_t count;
  LHashNode** array;
} LHashTable;


LHashNode* lach_LHashNode_create(char* key, void* value)
{
  if (key == NULL) {
    printf("Key is NULL.\n");
    return NULL;
  }
  
  if (value == NULL) {
    printf("Value is NULL.\n");
    return NULL;
  }
  
  LHashNode* node = (LHashNode*)calloc(1, sizeof(LHashNode));
  if (node == NULL) {
    printf("lach_HashNode* create_hashnode(char* key, void* value) could not allocate for HashNode.\n");
    return NULL;
  }

  // TODO: Should I copy this in with strdup()? Need to pass the size of the memory that value is pointing to
  node->key = key;
  node->value = value;
  return node;
}

LHashTable* lach_LHashTable_create(size_t capacity)
{
  if (capacity == 0) {
    capacity = LACH_HASHTABLE_DEFAULT_CAPACITY;
  }
  
  LHashTable* table = (LHashTable*)calloc(1, sizeof(LHashTable));
  if (table == NULL) {
    printf("Failed to allocate HashTable.\n");
    return NULL;
  }

  table->array = (LHashNode**)calloc(capacity, sizeof(LHashNode*));
  if (table->array == NULL) {
    printf("Failed to allocate HashTable array pointer.\n");
    return NULL;
  }

  table->capacity = capacity;
  table->count = 0;
  return table;
}

int lach_hash(LHashTable* table, char* key)
{
  if (table == NULL) {
    printf("Table is NULL.\n");
    return 1;
  }
  
  if (key == NULL) {
    printf("Key is NULL.\n");
    return 1;
  }
  
  // Djb2 algo
  size_t hash = 5381;
  int c;
  while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;
  }
  
  return hash % table->capacity;
}

int lach_LHashTable_insert(LHashTable* table, char* key, void* value)
{
  if (table == NULL) {
    printf("Table is NULL.\n");
    return 1;
  }
  
  if (key == NULL) {
    printf("Key is NULL.\n");
    return 1;
  }
  
  if (value == NULL) {
    printf("Value is NULL.\n");
    return 1;
  }
  
  if (table->count < table->capacity) {
    int i = lach_hash(table, key);
    LHashNode* node = lach_LHashNode_create(key, value);
    table->array[i] = node;
    table->count++;
    return 0;
  } else {
    printf("HashTable capacity reached, could not insert new key value pair.\n");
    return 1;
  }
}

void* lach_LHashTable_get(LHashTable* table, char* key)
{
  if (table == NULL) {
    printf("Table is NULL.\n");
    return NULL;
  }
  
  if (key == NULL) {
    printf("Key is NULL.\n");
    return NULL;
  }
  
  int i = lach_hash(table, key);
  LHashNode* node = table->array[i];
  if (node != NULL) {
    return node->value;
  } else {
    printf("HashTable value for key %s not found.\n", key);
    return NULL;
  }
}


int lach_LHashTable_free(LHashTable* table)
{
  if (table == NULL) {
    printf("HashTable is NULL.\n");
    return 1;
  }

  free(table->array);
  free(table);
  
  return 0;
}

#endif
