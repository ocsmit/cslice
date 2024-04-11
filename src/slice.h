#pragma once

#include "arena.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**! enum containing error codes */
typedef enum _slice_err {
  OK,

  MEMORY_ERROR,
  ACCESS_ERROR,
  LIMIT_ERROR,

  UNKNOWN_ERROR,
} slice_err;

static const char *VEC_ERROR_MESSAGE[] = {
    "OK", "MEMORY ERROR", "ACCESS ERROR", "LIMIT ERROR", "UNKOWN ERROR",
};

void slice_err_printf(slice_err *message);

// SLICE Implementation ========================================================

typedef struct _slice_t {
  size_t count;    // number of elements currently stored
  size_t capacity; // Max number of bits that can be stored
  void *ptr;       // pointer to data storage
} slice_t;

/**
 * @brief      Initialize members of slice
 *
 * @param      slice_t *
 *
 * @return     void
 */
void slice_init(slice_t *v);

/**
 * @brief      Allocate slice backed by any kind of buffer
 *
 * @details    Sets up a slice to hold n capacity bytes backed by the provided
 * buffer. Returns a slice_err type of either OK or MEMORY_ERROR. Used for
 * creating custom allocators for any slice type.
 *
 * @param      slice_t *
 * @param      size_t        total number of bytes in buffer
 * @param      void *        memory buffer
 *
 * @return     slice_err
 */
slice_err slice_buffer_alloc(slice_t *, size_t capacity, void *buffer);

/**
 * @brief      Allocate memory for a slice with 'count' elements
 *
 * @details    Allocates a memory buffer with 'realloc' for (count *
 * element_size) bytes
 *
 * @param      slice_t *
 * @param      size_t       count           number of elements to hold
 * @param      size_t       element_size    size in bytes of each element
 *
 * @return     slice_err
 */
slice_err slice_alloc(slice_t *, size_t count, size_t element_size);

/**
 * @brief      Allocates slice backed by memory arena
 *
 * @details    Reserves memory on arena for slice
 *
 * @param      slice_t *
 * @param      size_t       count           number of elements to hold
 * @param      arena_t *
 * @param      size_t       element_size    size in bytes of each element
 *
 * @return     slice_err
 */
slice_err slice_arena_alloc(slice_t *, size_t count, arena_t *a,
                            size_t element_size);

slice_err slice_free(slice_t *);

/**
 * @brief      Write data buffer to slice
 *
 * @details    Appends count * elemsize bytes from data buffer to a slice,
 * checking limits
 *
 * @param      slice_t *
 * @param      void   *data     data buffer of size (count * elemsize)
 * @param      size_t count     number of elements of size elemsize to write
 * into slice
 * @param      size_t elemsize  size of individual elements contained in data
 *
 * @return     return type
 */
slice_err slice_write(slice_t *, void *data, size_t count, size_t elemsize);

/**
 * @brief      pop last element on slice
 *
 * @param      slice_t *
 * @param      size_t count     number of elements to pop
 * @param      size_t stride    size of elements
 */
slice_err slice_pop(slice_t *, size_t count, size_t stride);

// STACK Interface ========================================================

// Defines a new slice type with a type-specific interface to
// slice_* functions and direct indexing.
// e.g. NEW_SLICE_TYPE(i8slice, int)
#define NEW_SLICE_TYPE(NAME, DTYPE)                                            \
  typedef struct _##NAME {                                                     \
    size_t count, capacity;                                                    \
    DTYPE *ptr;                                                                \
  } NAME##_t;                                                                  \
  NEW_SLICE_IMPL(NAME##_t, NAME, DTYPE)

// Define the implementation specific to slice type
#define NEW_SLICE_IMPL(SLICE_T, NAME, DATA_T)                                  \
  static_assert(sizeof(SLICE_T) >= sizeof(slice_t), "");                       \
  static_assert(                                                               \
      offsetof(SLICE_T, count) == offsetof(slice_t, count),                    \
      "memory offset of 'count' struct member does not match slice_t");        \
  static_assert(                                                               \
      offsetof(SLICE_T, capacity) == offsetof(slice_t, capacity),              \
      "memory offset of 'capacity' struct member does not match slice_t");     \
  static_assert(                                                               \
      offsetof(SLICE_T, ptr) == offsetof(slice_t, ptr),                        \
      "memory offset of 'ptr' struct member does not match slice_t");          \
  inline static void NAME##_init(SLICE_T *v) { slice_init((slice_t *)v); }     \
  inline static slice_err NAME##_alloc(SLICE_T *v, const size_t count) {       \
    return slice_alloc((slice_t *)v, count, sizeof(DATA_T));                   \
  }                                                                            \
  inline static slice_err NAME##_arena_alloc(SLICE_T *v, const size_t count,   \
                                             arena_t *a) {                     \
    return slice_arena_alloc((slice_t *)v, count, a, sizeof(DATA_T));          \
  }                                                                            \
  inline static slice_err NAME##_push(SLICE_T *v, DATA_T data) {               \
    return slice_write((slice_t *)v, (DATA_T[]){data}, 1, sizeof(DATA_T));     \
  }                                                                            \
  inline static slice_err NAME##_pop(SLICE_T *v) {                             \
    return slice_pop((slice_t *)v, 1, sizeof(DATA_T));                         \
  }                                                                            \
  inline static slice_err NAME##_push_arr(SLICE_T *v, const DATA_T data[],     \
                                          const size_t count) {                \
    return slice_write((slice_t *)v, (DATA_T *)data, count, sizeof(DATA_T));   \
  }                                                                            \
  inline static slice_err NAME##_free(SLICE_T *v) {                            \
    return slice_free((slice_t *)v);                                           \
  }                                                                            \
  inline static DATA_T NAME##_at(SLICE_T *v, size_t i) { return v->ptr[i]; }   \
  inline static DATA_T *NAME##_ptr_at(SLICE_T *v, size_t i) {                  \
    return &(v->ptr)[i];                                                       \
  }

// Init new stack
// NEW_SLICE_TYPE(int8s, int);
