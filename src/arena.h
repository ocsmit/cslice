#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DATA_ALIGNMENT
#define DATA_ALIGNMENT (2 * sizeof(void *))
#endif

typedef unsigned char byte;


/**
 * @brief      forward align address
 *
 * @details    finds the next aligned adress starting from ptr
 *
 * @param      uintptr_t ptr     address
 * @param      size_t    align   number of bytes for alignment
 *
 * @return     uintptr_t aligned address
 */
uintptr_t align_addr(uintptr_t ptr, size_t align);



// ARENA =======================================================================
typedef struct _arena {
  size_t curr_offset;
  size_t prev_offset;
  size_t capacity;
  byte *ptr;
} arena_t;

/**
 * @brief      Initialize memory backed arena
 *
 * @details    Sets all offsets to 0 and mem ptr to buffer
 *
 * @param      arena_t *
 * @param      void *buffer    memory buffer to back arena with
 * @param      size_t          size in bytes of buffer
 *
 * @return     void
 */
void arena_init(arena_t *a, void *buffer, size_t buffer_size);

/**
 * @brief      Initialize memory arena, dynamically setting up memory buffer
 *
 * @details    Sets up memory backed arena like 'arena_init' except allocating
 * and aligning the memory buffer is handled by the function.
 *
 * @param      arena_t *
 * @param      size_t size   number of bytes to allocate
 *
 * @return     void
 */
void arena_init_malloc(arena_t *a, size_t size);

/**
 * @brief      Allocates 'size' data to arena
 *
 * @details    The address of the data will be set to the nearest forward
 * aligned address.
 *
 * @param      arena_t *
 * @param      size_t    size    number of bytes to reserve
 * @param      size_t    align   number of bytes for alignment
 *
 * @return     void *    starting address of reserved memory
 */
void *arena_alloc(arena_t *a, size_t size, size_t align);

/**
 * @brief      Reset arena
 *
 * @details    Offsets are set to zero and memory is zeroed
 *
 * @param      arena_t *a
 *
 * @return     void
 */
void arena_free(arena_t *a);
