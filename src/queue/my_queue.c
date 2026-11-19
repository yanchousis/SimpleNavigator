#include "my_queue.h"

my_queue* queue() {
  my_queue* q = malloc(sizeof(my_queue));

  if (q) {
    q->size = 0;
    q->capacity = 0;
    q->arr = NULL;
  }

  return q;
}

void queue_destroy(my_queue** q) {
  if (!q || !(*q)) return;

  free((*q)->arr);
  free(*q);

  *q = NULL;
}

void queue_print(my_queue* q, char* msg) {
  if (!q) return;
  if (!q->arr) {
    printf("empty queue\n");
    return;
  }

  for (int i = 0; i < q->size; ++i) {
    printf("%s\t%d: %u\n", msg, i + 1, q->arr[i]);
  }
}

void queue_allocate(my_queue* q) {
  if (!q) return;

  size_t new_cap = (!q->capacity) ? 32 : q->capacity * 2;

  unsigned* tmp = realloc(q->arr, sizeof(unsigned) * new_cap);
  if (tmp) {
    q->arr = tmp;
    q->capacity = new_cap;
  }
}

void queue_push(my_queue* q, unsigned value) {
  if (!q) return;

  if (q->size >= q->capacity) {
    queue_allocate(q);
  }

  for (int i = q->size - 1; i >= 0; --i) {
    q->arr[i + 1] = q->arr[i];
  }
  q->arr[0] = value;

  q->size++;
}

unsigned queue_pop(my_queue* q) {
  if (!q || !q->arr || !q->size) return 0;

  return q->arr[--(q->size)];
}

unsigned queue_front(my_queue* q) {
  if (!q || !q->arr || !q->size) return 0;

  return q->arr[0];
}

unsigned queue_back(my_queue* q) {
  if (!q || !q->arr || !q->size) return 0;

  return q->arr[q->size - 1];
}
