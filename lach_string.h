#ifndef LACH_STRING_H
#define LACH_STRING_H


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "lach_vector.h"

#ifdef LACH_DEBUG
#define LACH_DEBUG_PRINT(fmt, ...) \
    do { \
        time_t now; \
        struct tm *local_time; \
        char timestamp[20]; \
        time(&now); \
        local_time = localtime(&now); \
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time); \
        fprintf(stderr, "[%s] [ERROR] <%s:%d> %s: ", timestamp, __FILE__, __LINE__, __func__); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define LACH_DEBUG_PRINT(fmt, ...) \
    do {} while (0)
#endif



typedef struct {
  size_t len;   // Also equals the index of the null terminator, which is not counted in the length
  char* bytes;  // Will always be null terminated
} LString;


// Create a LString from a null terminate array of chars
LString* lach_LString_create_from_term(char* bytes)
{
  if (bytes == NULL) {
    LACH_DEBUG_PRINT("String create from null failed.\n");
    return NULL;
  }

  LString* str = (LString*)calloc(1, sizeof(LString));
  if (str == NULL) {
    LACH_DEBUG_PRINT("String struct allocation failed for bytes %s.\n", bytes);
  }

  int i = 0;
  char c = bytes[i];
  while (c != '\0') {
    LACH_DEBUG_PRINT("Searching for null termination in bytes: [%s], index [%d] checked.\n", bytes, i);
    i++;
    c = bytes[i];
  }

  str->bytes = (char*)calloc(i + 1, sizeof(char));
  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String bytes allocation failed for bytes %s.\n", bytes);
  }

  memcpy(str->bytes, bytes, i + 1);
  str->len = i;
  return str;
}

// Create a LString from an array of chars, where len is just the # of chars
// not including the null terminator.
LString* lach_LString_create_from_len(char* bytes, size_t len)
{
  if (bytes == NULL) {
    LACH_DEBUG_PRINT("String create from null failed.\n");
    return NULL;
  }

  // Check actually is a null at len + 1, being the null term index
  // TODO: Could add one, or have a diff function that adds one?
  //        Also want to move to 1 create function that deals with all this complexity
  if (bytes[len] != '\0') {
    LACH_DEBUG_PRINT("Bytes [%s] did not have null termination at index [%ld].\n", bytes, len);
    return NULL;
  }

  LString* str = (LString*)calloc(1, sizeof(LString));
  if (str == NULL) {
    LACH_DEBUG_PRINT("String struct allocation failed for bytes %s.\n", bytes);
  }

  str->bytes = (char*)calloc(len + 1, sizeof(char));
  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String bytes allocation failed for bytes %s.\n", bytes);
  }

  memcpy(str->bytes, bytes, len + 1);
  str->len = len;
  return str;
}

// Copy an LString
LString* lach_LString_copy(LString* str)
{
  if (str == NULL) {
    LACH_DEBUG_PRINT("String copy from null failed.\n");
    return NULL;
  }

  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String copy from null bytes failed.\n");
    return NULL;
  }

  LString* copy = lach_LString_create_from_term(str->bytes);
  if (copy == NULL) {
    LACH_DEBUG_PRINT("Call to String create failed.\n");
    return NULL;
  }

  return copy;
}

// Combining string b into string a strings together.
int lach_LString_concat(LString* strA, LString* strB)
{
  if (strA == NULL) {
    LACH_DEBUG_PRINT("String A is null.\n");
    return 1;
  }

  if (strA->bytes == NULL) {
    LACH_DEBUG_PRINT("String A bytes is null.\n");
    return 1;
  }
  
  if (strB == NULL) {
    LACH_DEBUG_PRINT("String B is null.\n");
    return 1;
  }

  if (strB->bytes == NULL) {
    LACH_DEBUG_PRINT("String B bytes is null.\n");
    return 1;
  }

  strA->bytes = (char*)realloc(strA->bytes, (strA->len + (strB->len + 1)) * sizeof(char));
  if (strA->bytes == NULL) {
    LACH_DEBUG_PRINT("String realloc failed.\n");
    return 1;
  }
  
  memcpy(&strA->bytes[strA->len + 1], strB->bytes, strB->len + 1);
  return 0;
}

// Extracting a portion of a string based on its position.
int lach_LString_substring(LString* str, size_t start_i, size_t end_i);

// Comparing two strings to determine equality.
bool lach_LString_comp(LString* strA, LString* strB);

// Convert to lower case.
int lach_LString_lower(LString* str);

// Convert to upper case.
int lach_LString_upper(LString* str);

// Finding the position of the first matching substring within a string.
int lach_LString_forstr(LString* strA, LString* strB);
// 
// Finding the position of the first matching char within a string.
int lach_LString_forchar(LString* strA, char c);

// Removing leading and trailing whitespace characters from a string.
int lach_LString_trim(LString* str);

// Breaking a string into a substring based on a delimiter.
int lach_LString_split(char c);





#endif
