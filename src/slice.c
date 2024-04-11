#include "slice.h"
#include "arena.h"

void slice_err_printf(slice_err *message) {
  if (message && *message > OK && *message < UNKNOWN_ERROR) {
    printf("failed with error : %s\n", VEC_ERROR_MESSAGE[*message]);
  }
}

void slice_init(slice_t *v) {
  v->count = 0;
  v->capacity = 0;
  v->ptr = NULL;
}

slice_err slice_buffer_alloc(slice_t *v, size_t capacity, void *buffer) {
  if (!buffer)
    return MEMORY_ERROR;

  v->count = 0;
  v->capacity = capacity;
  v->ptr = buffer;
  return OK;
}

slice_err slice_alloc(slice_t *v, size_t count, size_t element_size) {
  size_t c = (count * element_size);

  // Check for overflow
  size_t max_items = SIZE_MAX / element_size;
  if (max_items < c)
    return MEMORY_ERROR;

  // allocate space
  void *p = realloc(v->ptr, c);
  slice_buffer_alloc(v, c, p);
  return OK;
}

slice_err slice_arena_alloc(slice_t *v, size_t count, arena_t *a,
                            size_t element_size) {

  size_t c = (count * element_size);

  // Check for overflow
  size_t max_items = SIZE_MAX / element_size;
  if (max_items < c)
    return MEMORY_ERROR;

  // allocate space
  void *p = arena_alloc(a, c, DATA_ALIGNMENT);
  return slice_buffer_alloc(v, c, p);
}

slice_err slice_free(slice_t *v) {
  if (!(v->ptr))
    return MEMORY_ERROR;

  free(v->ptr);
  slice_init(v);
  return OK;
}

slice_err slice_write(slice_t *v, void *data, size_t count, size_t elemsize) {

  // check if there is still room in vector
  if ((v->capacity - v->count) < (elemsize * count)) {
    // printf("ERROR: Trying to write more data than allocated for in fixed "
    //        "vector\n");
    return LIMIT_ERROR;
  }

  void *dst = &((char *)v->ptr)[v->count];
  if (!dst)
    return MEMORY_ERROR;

  memcpy(dst, data, elemsize * count);
  v->count += (count * elemsize);
  return OK;
}

slice_err slice_pop(slice_t *v, size_t count, size_t stride) {
  if (v->count < (count * stride))
    return MEMORY_ERROR;

  v->count -= (count * stride);
  return OK;
}
