#ifdef __cplusplus
extern "C" {
#endif

#define LACH_ALL_H
// #define LACH_LIST_H
// #define LACH_HASHTABLE_H
// #define LACH_LOG_H
// #define LACH_STRING_H
// #define LACH_VECTOR_H

#ifdef LACH_DEBUG
#define LACH_DEBUG_PRINT(fmt, ...)                                             \
  do {                                                                         \
    time_t now;                                                                \
    struct tm *local_time;                                                     \
    char timestamp[20];                                                        \
    time(&now);                                                                \
    local_time = localtime(&now);                                              \
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);   \
    fprintf(stderr, "[%s] [ERROR] <%s:%d> %s: ", timestamp, __FILE__,          \
            __LINE__, __func__);                                               \
    fprintf(stderr, fmt, ##__VA_ARGS__);                                       \
  } while (0)
#else
#define LACH_DEBUG_PRINT(fmt, ...)                                             \
  do {                                                                         \
  } while (0)
#endif

/******************************************************************************/
// Hash table
/******************************************************************************/

#if defined(LACH_HASHTABLE_H) || defined(LACH_ALL_H)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LACH_HASHTABLE_DEFAULT_CAPACITY
#define LACH_HASHTABLE_DEFAULT_CAPACITY 255
#endif

typedef struct {
  char *key;
  void *value;
} LHashNode;

// TODO: Use a vector?
typedef struct {
  size_t capacity;
  size_t count;
  LHashNode **array;
} LHashTable;

LHashNode *lach_LHashNode_create(char *key, void *value) {
  if (key == NULL) {
    printf("Key is NULL.\n");
    return NULL;
  }

  if (value == NULL) {
    printf("Value is NULL.\n");
    return NULL;
  }

  LHashNode *node = (LHashNode *)calloc(1, sizeof(LHashNode));
  if (node == NULL) {
    printf("lach_HashNode* create_hashnode(char* key, void* value) could not "
           "allocate for HashNode.\n");
    return NULL;
  }

  // TODO: Should I copy this in with strdup()? Need to pass the size of the
  // memory that value is pointing to
  node->key = key;
  node->value = value;
  return node;
}

LHashTable *lach_LHashTable_create(size_t capacity) {
  if (capacity == 0) {
    capacity = LACH_HASHTABLE_DEFAULT_CAPACITY;
  }

  LHashTable *table = (LHashTable *)calloc(1, sizeof(LHashTable));
  if (table == NULL) {
    printf("Failed to allocate HashTable.\n");
    return NULL;
  }

  table->array = (LHashNode **)calloc(capacity, sizeof(LHashNode *));
  if (table->array == NULL) {
    printf("Failed to allocate HashTable array pointer.\n");
    return NULL;
  }

  table->capacity = capacity;
  table->count = 0;
  return table;
}

int lach_hash(LHashTable *table, char *key) {
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
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % table->capacity;
}

int lach_LHashTable_insert(LHashTable *table, char *key, void *value) {
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
    LHashNode *node = lach_LHashNode_create(key, value);
    table->array[i] = node;
    table->count++;
    return 0;
  } else {
    printf(
        "HashTable capacity reached, could not insert new key value pair.\n");
    return 1;
  }
}

void *lach_LHashTable_get(LHashTable *table, char *key) {
  if (table == NULL) {
    printf("Table is NULL.\n");
    return NULL;
  }

  if (key == NULL) {
    printf("Key is NULL.\n");
    return NULL;
  }

  int i = lach_hash(table, key);
  LHashNode *node = table->array[i];
  if (node != NULL) {
    return node->value;
  } else {
    printf("HashTable value for key %s not found.\n", key);
    return NULL;
  }
}

int lach_LHashTable_free(LHashTable *table) {
  if (table == NULL) {
    printf("HashTable is NULL.\n");
    return 1;
  }

  free(table->array);
  free(table);

  return 0;
}

#endif // LACH_HASHTABLE_H

/******************************************************************************/
// List
/******************************************************************************/

#if defined(LACH_LIST_H) || defined(LACH_ALL_H)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LListNode LListNode;

struct LListNode {
  void *data;      // The pointer to the data stored by the list node.
  LListNode *next; // The next node in the list.
  LListNode *prev; // The previous node in the list.
};

typedef struct {
  size_t count;    // The number of list nodes in the list.
  size_t size;     // The size of data type stored by each list node.
  LListNode *head; // The head of the list.
  LListNode *tail; // The tail of the list.
} LList;

// Logging function pointer type
typedef void (*lach_LList_LoggerCallback)(const char *format, ...);

// Logging callback global variable
static lach_LList_LoggerCallback lach_LList_logger = NULL;

/*
 * Set the logging callback
 * @param callback User defined logging callback
 */
void lach_LList_setLogger(lach_LList_LoggerCallback callback) {
  lach_LList_logger = callback;
}

/*
 * Create a doubly-linked list.
 *
 * @param size The size of the data type to be stored by each list node.
 * @return LList* A pointer to the newly allocated list.
 */
LList *lach_LList_create(size_t size) {
  LList *list = (LList *)calloc(1, sizeof(LList));
  if (list == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Failed to allocate memory for LList.");
    }
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
LListNode *lach_LListNode_create(void *data, size_t size) {
  if (data == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Data is NULL.");
    }
    return NULL;
  }

  LListNode *node = (LListNode *)calloc(1, sizeof(LListNode));
  if (node == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Failed to allocate memory for ListNode.");
    }
    return NULL;
  }

  node->data = data;
  memcpy(node->data, data, size);

  return node;
}

/*
 * Add to tail of doubly-linked list.
 */
int lach_LList_append(LList *list, void *data) {
  if (list == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("List is NULL.");
    }
    // TODO: LList error codes
    return 1;
  }

  if (data == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Data is NULL.");
    }
    return 1;
  }

  LListNode *new_node = lach_LListNode_create(data, list->size);
  if (new_node == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Failed to append new ListNode.");
    }
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
int lach_LList_prepend(LList *list, void *data) {
  if (list == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("List is NULL.");
    }
    // TODO: LList error codes
    return 1;
  }

  if (data == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Data is NULL.");
    }
    return 1;
  }

  LListNode *new_node = lach_LListNode_create(data, list->size);
  if (new_node == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Failed to append new ListNode.");
    }
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

void *lach_LList_at(LList *list, size_t index) {
  if (list == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("List is NULL.");
    }
    return NULL;
  }

  if (index >= list->count) {
    if (lach_LList_logger) {
      lach_LList_logger("Index %d must be less than the list count %zu.", index,
                        list->count);
    }
    return NULL;
  }

  // TODO: Is there a better iterator way than this?
  size_t i = 0;
  for (LListNode *node = list->head; node != NULL; node = node->next) {
    if (i == index) {
      return node;
    } else {
      i++;
    }
  }

  if (lach_LList_logger) {
    lach_LList_logger("Could not find ListNode at index %d. List ended at "
                      "index %d with count.",
                      index, i, list->count);
  }

  return NULL;
}

int lach_LList_insert(LList *list, size_t index, void *data) {
  if (list == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("List is NULL.");
    }
    return 1;
  }

  if (index >= list->count) {
    if (lach_LList_logger) {
      lach_LList_logger("Index %d must be less than the list count %zu.", index,
                        list->count);
    }
    return 1;
  }

  LListNode *new_node = lach_LListNode_create(data, list->size);
  if (new_node == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Failed to append new ListNode.");
    }
    return 1;
  }

  if (list->count == 0) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    // TODO: Is there a better iterator way than this?
    size_t i = 0;
    for (LListNode *node = list->head; node != NULL; node = node->next) {
      if (i == index) {
        if (node->prev) {
          node->prev->next = new_node;
          new_node->prev = node->prev;
        } else {
          // TODO: was trying to be clever with taking care of tail and head
          // edge cases with middle but if 0 index or count - 1 index specified
          // im not sure if this will work... I'm tired w
          list->tail = new_node;
        }
      }
    }
    if (lach_LList_logger) {
      lach_LList_logger(
          "Could not find index %d. List ended at index %d with count.", index,
          i, list->count);
    }
  }

  return 1;
}

int lach_LListNode_free(LListNode *head) {
  if (head == NULL) {
    if (lach_LList_logger) {
      lach_LList_logger("Head is NULL.");
    }
    return 1;
  }

  LListNode *curr = head;
  while (curr->next != NULL) {
    LListNode *prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(curr);

  return 0;
}

#endif // LACH_LIST_H

/******************************************************************************/
// Log
/******************************************************************************/

#if defined(LACH_LOG_H) || defined(LACH_ALL_H)

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

typedef enum {
  // Indicates that the message should always be logged regardless of other
  // settings.
  LLOG_ALWAYS,

  // Severe errors that will lead the application to abort or terminate. These
  // are critical events that usually signify the need for immediate attention.
  LLOG_FATAL,

  // Error events that might allow the application to continue running, but
  // something failed or went wrong in the process.
  LLOG_ERROR,

  // Potentially harmful situations that aren't necessarily errors but may
  // require attention. These indicate that something unexpected happened but
  // the application is still running.
  LLOG_WARN,

  // Informational messages that highlight the progress of the application in a
  // general sense, such as startup or shutdown messages.
  LLOG_INFO,

  // General debugging information useful during development, but not usually
  // needed in production environments.
  LLOG_DEBUG,

  // Detailed debug information, typically used for tracing the flow of the
  // application at a fine-grained level.
  LLOG_TRACE
} LLogLevel;

const char *lach_log_level_str[] = {"ALWAYS", "FATAL", "ERROR", "WARN",
                                    "INFO",   "DEBUG", "TRACE"};

void lach_log(LLogLevel level, const char *format, ...) {
  time_t t = time(NULL);

  fprintf(stderr, "[%s] [%s] ", asctime(localtime(&t)),
          lach_log_level_str[level]);

  // Handle the variable argument list
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

#endif // LACH_LOG_H

/******************************************************************************/
// String
/******************************************************************************/

#if defined(LACH_STRING_H) || defined(LACH_ALL_H)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Inner dependency
#define LACH_VECTOR_H

typedef struct {
  size_t len;  // Also equals the index of the null terminator, which is not
               // counted in the length
  char *bytes; // Will always be null terminated
} LString;

// Create a LString from a null terminate array of chars
LString *lach_LString_create_from_term(char *bytes) {
  if (bytes == NULL) {
    LACH_DEBUG_PRINT("String create from null failed.\n");
    return NULL;
  }

  LString *str = (LString *)calloc(1, sizeof(LString));
  if (str == NULL) {
    LACH_DEBUG_PRINT("String struct allocation failed for bytes %s.\n", bytes);
  }

  int i = 0;
  char c = bytes[i];
  while (c != '\0') {
    LACH_DEBUG_PRINT(
        "Searching for null termination in bytes: [%s], index [%d] checked.\n",
        bytes, i);
    i++;
    c = bytes[i];
  }

  str->bytes = (char *)calloc(i + 1, sizeof(char));
  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String bytes allocation failed for bytes %s.\n", bytes);
  }

  memcpy(str->bytes, bytes, i + 1);
  str->len = i;
  return str;
}

// Create a LString from an array of chars, where len is just the # of chars
// not including the null terminator.
LString *lach_LString_create_from_len(char *bytes, size_t len) {
  if (bytes == NULL) {
    LACH_DEBUG_PRINT("String create from null failed.\n");
    return NULL;
  }

  // Check actually is a null at len + 1, being the null term index
  // TODO: Could add one, or have a diff function that adds one?
  //        Also want to move to 1 create function that deals with all this
  //        complexity
  if (bytes[len] != '\0') {
    LACH_DEBUG_PRINT(
        "Bytes [%s] did not have null termination at index [%ld].\n", bytes,
        len);
    return NULL;
  }

  LString *str = (LString *)calloc(1, sizeof(LString));
  if (str == NULL) {
    LACH_DEBUG_PRINT("String struct allocation failed for bytes %s.\n", bytes);
  }

  str->bytes = (char *)calloc(len + 1, sizeof(char));
  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String bytes allocation failed for bytes %s.\n", bytes);
  }

  memcpy(str->bytes, bytes, len + 1);
  str->len = len;
  return str;
}

// Copy an LString
LString *lach_LString_copy(LString *str) {
  if (str == NULL) {
    LACH_DEBUG_PRINT("String copy from null failed.\n");
    return NULL;
  }

  if (str->bytes == NULL) {
    LACH_DEBUG_PRINT("String copy from null bytes failed.\n");
    return NULL;
  }

  LString *copy = lach_LString_create_from_term(str->bytes);
  if (copy == NULL) {
    LACH_DEBUG_PRINT("Call to String create failed.\n");
    return NULL;
  }

  return copy;
}

// Combining string b into string a strings together.
int lach_LString_concat(LString *strA, LString *strB) {
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

  strA->bytes = (char *)realloc(strA->bytes,
                                (strA->len + (strB->len + 1)) * sizeof(char));
  if (strA->bytes == NULL) {
    LACH_DEBUG_PRINT("String realloc failed.\n");
    return 1;
  }

  memcpy(&strA->bytes[strA->len + 1], strB->bytes, strB->len + 1);
  return 0;
}

// Extracting a portion of a string based on its position.
int lach_LString_substring(LString *str, size_t start_i, size_t end_i);

// Comparing two strings to determine equality.
bool lach_LString_comp(LString *strA, LString *strB);

// Convert to lower case.
int lach_LString_lower(LString *str);

// Convert to upper case.
int lach_LString_upper(LString *str);

// Finding the position of the first matching substring within a string.
int lach_LString_forstr(LString *strA, LString *strB);
//
// Finding the position of the first matching char within a string.
int lach_LString_forchar(LString *strA, char c);

// Removing leading and trailing whitespace characters from a string.
int lach_LString_trim(LString *str);

// Breaking a string into a substring based on a delimiter.
int lach_LString_split(char c);

#endif // LACH_STRING_H

/******************************************************************************/
// Vector
/******************************************************************************/

#if defined(LACH_VECTOR_H) || defined(LACH_ALL_H)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LACH_VECTOR_DEFAULT_CAPACITY 255

typedef struct {
  size_t count;
  size_t capacity;
  void **array;
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
LVector *lach_LVector_create(size_t capacity) {
  if (capacity == 0) {
    capacity = LACH_VECTOR_DEFAULT_CAPACITY;
  }

  LVector *vec = (LVector *)calloc(1, sizeof(LVector));
  if (vec == NULL) {
    LACH_DEBUG_PRINT("Vector allocation failed.\n");
    return NULL;
  }

  vec->array = (void **)calloc(capacity, sizeof(void *));
  if (vec->array == NULL) {
    LACH_DEBUG_PRINT("Vector array allocation failed. Capacity = %ld.\n",
                     capacity);
    return NULL;
  }

  vec->capacity = capacity;
  vec->count = 0;
  return vec;
}

/**
 *
 * @brief This function appends to a vector
 *
 * @param vec The vector to append to
 * @param data The data to append to the vector
 *
 * @return 1 for failure (vector is null, data ptr is null,
 * reallocation failed), 0 for success
 *
 */
int lach_LVector_append(LVector *vec, void *data) {
  if (vec == NULL) {
    LACH_DEBUG_PRINT("Vector is NULL\n");
    return 1;
  }

  if (data == NULL) {
    LACH_DEBUG_PRINT("Data is NULL\n");
    return 1;
  }

  if (vec->count++ > vec->capacity) {
    vec->capacity *= 2;
    vec->array = (void **)realloc(vec->array, vec->capacity * sizeof(void *));
    if (vec->array == NULL) {
      LACH_DEBUG_PRINT(
          "Vector array reallocation failed. Count = %ld and Capacity = %ld.\n",
          vec->count, vec->capacity);
      return 1;
    }
  }

  vec->array[vec->count - 1] = data;
  return 0;
}

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
void *lach_LVector_get(LVector *vec, size_t index) {
  if (vec == NULL) {
    LACH_DEBUG_PRINT("Vector is NULL.\n");
    return NULL;
  }

  if (index >= vec->capacity) {
    LACH_DEBUG_PRINT(
        "Requested index is out of bounds for vector with %ld capacity.\n",
        vec->capacity);
    return NULL;
  }

  return vec->array[index];
}

/**
 *
 * @brief This function frees the vector array and vector
 *
 * @param vec The vector to free
 *
 * @return 1 for failure (vector is null), 0 for success
 *
 */
int lach_LVector_free(LVector *vec) {
  if (vec == NULL) {
    LACH_DEBUG_PRINT("Vector is NULL.\n");
    return 1;
  }

  free(vec->array);
  free(vec);
  return 0;
}

#endif // LACH_VECTOR_H

#ifdef __cplusplus
}
#endif
