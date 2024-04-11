#include "arena.h"
#include "slice.h"

// Init new stack
NEW_SLICE_TYPE(int8s, int)

// ============================================================================
int main(int argc, char *argv[]) {
  int data[] = {1, 2, 44};

  printf("testing stack i8\n");

  int8s_t vv;
  int8s_init(&vv);

  slice_err status = int8s_alloc(&vv, 10);

  status = int8s_push(&vv, 8);
  slice_err_printf(&status);

  int8s_push(&vv, 237);
  int8s_push_arr(&vv, data, 3);
  int8s_push(&vv, 99);
  int8s_push_arr(&vv, data, 3);
  int8s_push(&vv, -99);
  for (size_t i = 0; i < 10; ++i)
    printf("%lu : %d\n", i, vv.ptr[i]);
  printf("%lu, %lu\n", vv.count, vv.capacity);

  status = int8s_push(&vv, 8);

  printf("%lu, %lu\n", vv.count, vv.capacity);
  slice_err_printf(&status);

  printf("pop\n");
  int8s_pop(&vv);
  printf("%lu, %lu\n", vv.count, vv.capacity);
  printf("push\n");
  status = int8s_push(&vv, 8);
  slice_err_printf(&status);
  printf("%lu, %lu\n", vv.count, vv.capacity);

  int8s_free(&vv);

  printf("DONE\n");
  printf("\ntesting arena\n");

  arena_t a;
  arena_init_malloc(&a, sizeof(int8s_t) + 40);
  printf("arena cap: %lu\n", a.capacity);

  status = int8s_arena_alloc(&vv, 10, &a);
  slice_err_printf(&status);

  printf("arena offset: %lu\n", a.curr_offset);
  printf("stack cap: %lu\n", vv.capacity);

  status = int8s_push(&vv, 8);
  slice_err_printf(&status);
  int8s_push(&vv, 237);
  int8s_push_arr(&vv, data, 3);
  int8s_push(&vv, 99);
  int8s_push_arr(&vv, data, 3);
  int8s_push(&vv, -99);
  for (size_t i = 0; i < 10; ++i)
    printf("%lu : %d\n", i, vv.ptr[i]);
  printf("%lu, %lu\n", vv.count, vv.capacity);

  int *d = int8s_ptr_at(&vv, 3);
  *d = 9999;


  for (size_t i = 0; i < 10; ++i)
    printf("%lu : %d\n", i, vv.ptr[i]);

  arena_free(&a);
  int8s_free(&vv);

  return 0;
}
