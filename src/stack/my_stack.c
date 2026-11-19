#include "my_stack.h"

my_stack* stack() {
  my_stack* s = malloc(sizeof(my_stack));

  if (s) {
    s->size = 0;
    s->capacity = 0;
    s->arr = NULL;
  }

  return s;
}

void stack_destroy(my_stack** s) {
  if (!s || !(*s)) return;

  free((*s)->arr);
  free(*s);

  *s = NULL;
}

void stack_print(my_stack* s, char* msg) {
  if (!s) return;
  if (!s->arr) {
    printf("empty stack\n");
    return;
  }

  for (int i = s->size - 1; i >= 0; --i) {
    printf("%s\t%d: %u\n", msg, s->size - i, s->arr[i]);
  }
}

void stack_allocate(my_stack* s) {
  if (!s) return;

  size_t new_cap = (!s->capacity) ? 32 : s->capacity * 2;

  unsigned* tmp = realloc(s->arr, sizeof(unsigned) * new_cap);
  if (tmp) {
    s->arr = tmp;
    s->capacity = new_cap;
  }
}

void stack_push(my_stack* s, unsigned value) {
  if (!s) return;

  if (s->size >= s->capacity) {
    stack_allocate(s);
  }

  s->arr[s->size++] = value;
}

unsigned stack_pop(my_stack* s) {
  if (!s || !s->arr || !s->size) return 0;

  return s->arr[--(s->size)];
}

unsigned stack_top(my_stack* s) {
  if (!s || !s->arr || !s->size) return 0;

  return s->arr[s->size - 1];
}
