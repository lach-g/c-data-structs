#ifndef LACH_HASHTABLE_H
#define LACH_HASHTABLE_H


#include <stdlib.h>
#include <stdio.h>

typedef struct {
  char* key;
  void* value;
} HashNode;

// TODO: Use a vector?
typedef struct {
  size_t capacity;
  size_t count;
  HashNode** array;
} HashTable;


HashTable* lach_HashTable_create(size_t capacity);
int lach_HashTable_insert(HashTable* table, char* key, void* value);
void* lach_HashTable_get(HashTable* table, char* key);


//***************************************************************
// 
// Implementations
// 
//***************************************************************


HashNode* create_HashNode(char* key, void* value)
{
  // TODO: Input checks for functions
  HashNode* node = (HashNode*)calloc(1, sizeof(HashNode));
  if (node == NULL) {
    printf("HashNode* create_hashnode(char* key, void* value) could not allocate for HashNode.\n");
    return NULL;
  }

  // TODO: Should I copy this in with strdup()? Need to pass the size of the memory that value is pointing to
  node->key = key;
  node->value = value;
  return node;
}

HashTable* lach_HashTable_create(size_t capacity)
{
  HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));
  if (table == NULL) {
    printf("HashTable* create_hashtable(size_t capacity) could not allocate for HashTable.\n");
    return NULL;
  }

  table->array = (HashNode**)calloc(capacity, sizeof(HashNode*));
  if (table->array == NULL) {
    printf("HashTable* create_hashtable(size_t capacity) could not allocate for HashTable array pointer.\n");
    return NULL;
  }

  table->capacity = capacity;
  table->count = 0;
  return table;
}

int hash(HashTable* table, char* key)
{
    unsigned long val = 0;
    int c;
  
    while ((c = *key++)) {
        val = c + (val << 6) + (val << 16) - val;
    }
  
    return val % table->capacity;
}

int lach_HashTable_insert(HashTable* table, char* key, void* value)
{
  if (table->count < table->capacity) {
    int i = hash(table, key);
    HashNode* node = create_HashNode(key, value);
    table->array[i] = node;
    table->count++;
    return 0; // TODO: Error codes?
  } else {
    printf("HashTable capacity reached, could not insert new key value pair.\n");
    return 1;
  }
}

void* lach_HashTable_get(HashTable* table, char* key)
{
  int i = hash(table, key);
  HashNode* node = table->array[i];
  if (node != NULL) {
    return node->value;
  } else {
    printf("HashTable value for key %s not found.\n", key);
    return NULL;
  }
}


// TODO: FREE!

#endif
