#ifndef LACH_VECTOR_H
#define LACH_VECTOR_H


#include <stdlib.h>
#include <stdio.h>

#define LACH_VECTOR_DEFAULT_CAPACITY 255

typedef struct {
  size_t count;
  size_t capacity;
  void** array;
} LVector;

/**
*
* @brief This function creates a vector
*
* @param capacity The size of the initial array within the vector
* 
* @return NULL if failure, otherwise a pointer to the allocated vector
*
*/
LVector* lach_LVector_create(size_t capacity);

/**
*
* @brief This function appends to a vector
*
* @param vec The vector to append to
* @param data The data to append to the vector
* 
* @return 1 for failure (reallocation failed), 0 for success
*
*/
int lach_LVector_append(LVector* vec, void* data);

/**
*
* @brief This function gets the data at the specified index
*
* @param vec The vector to index
* @param index The index to get
* 
* @return NULL if failure (vector is null, index >= capacity of vector)
* otherwise a pointer to the data
*
*/
void* lach_LVector_get(LVector* vec, size_t index);

/**
*
* @brief This function frees the vector array and vector
*
* @param vec The vector to free
* 
* @return 1 for failure (vector is null), 0 for success
*
*/
int lach_LVector_free(LVector* vec);

//***************************************************************
// 
// Implementations
// 
//***************************************************************

LVector* lach_LVector_create(size_t capacity)
{
  if (capacity == 0) {
    capacity = LACH_VECTOR_DEFAULT_CAPACITY;
  }
  
  LVector* vec = (LVector*)calloc(1, sizeof(LVector));
  if (vec == NULL) {
    printf("Vector allocation failed.\n"); // TODO: Consistent error handling
    return NULL;
  }

  vec->array = (void**)calloc(capacity, sizeof(void*));
  if (vec->array == NULL) {
    printf("Vector array allocation failed. Capacity = %ld.\n", capacity);
    return NULL;
  }

  vec->capacity = capacity;
  vec->count = 0;
  return vec;
}

int lach_LVector_append(LVector* vec, void* data)
{
  if (vec->count++ > vec->capacity) {
    vec->capacity *= 2;
    vec->array = (void**)realloc(vec->array, vec->capacity * sizeof(void*));
    if (vec->array == NULL) {
      printf("Vector array reallocation failed. Count = %ld and Capacity = %ld.\n", vec->count, vec->capacity);
      return 1;
    }
  }

  vec->array[vec->count - 1] = data;
  return 0;
}

void* lach_LVector_get(LVector* vec, size_t index)
{
  if (vec == NULL) {
    printf("Vector is NULL.\n");
    return NULL;
  }
  
  if (index >= vec->capacity) {
    printf("Requested index is out of bounds for vector with %ld capacity.\n", vec->capacity);
    return NULL;
  }

  return vec->array[index];
}

int lach_LVector_free(LVector* vec)
{
  if (vec == NULL) {
    printf("Vector is NULL.\n");
    return 1;
  }

  free(vec->array);
  free(vec);
  return 0;
}


#endif
