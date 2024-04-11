#include "arena.h"

bool power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

uintptr_t align_addr(uintptr_t ptr, size_t align) {
  uintptr_t p, a, m;

  assert(power_of_two(align));

  p = ptr;
  a = (uintptr_t)align;
  m = p & (a - 1); // modulo

  if (m != 0) {
    // if addr is not aligned, push to next aligned value
    p += (a - m);
  }

  return p;
}

void arena_init(arena_t *a, void *buffer, size_t buffer_size) {
  a->curr_offset = 0;
  a->prev_offset = 0;
  a->capacity = buffer_size;
  a->ptr = (byte *)buffer;
}

void arena_init_malloc(arena_t *a, size_t size) {
  uintptr_t offset = align_addr(size, DATA_ALIGNMENT);
  byte *b = malloc(offset);
  if (b == NULL) return;
  arena_init(a, b, offset);
}


void *arena_alloc(arena_t *a, size_t size, size_t align) {
  // align offset forward
  uintptr_t curr_ptr = (uintptr_t)a->ptr + (uintptr_t)a->curr_offset;
  uintptr_t offset = align_addr(curr_ptr, align);
  offset -= (uintptr_t)a->ptr; // relative

  // check capacity
  if ((offset + size) <= a->capacity) {
    void *ptr = &a->ptr[offset];
    a->prev_offset = offset;
    a->curr_offset = offset + size;

    // zero out mem
    memset(ptr, 0, size);
    return ptr;
  }

  return NULL;

}

void arena_free(arena_t *a) {
  a->curr_offset = 0;
  a->prev_offset = 0;
  memset(a->ptr, 0, a->capacity); // zero out mem
}
