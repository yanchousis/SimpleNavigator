#include "heap.h"

void heap_init(min_heap_t* heap) {
  heap->data = malloc(INITIAL_HEAP_CAPACITY * sizeof(heap_node_t));
  heap->size = 0;
  heap->capacity = INITIAL_HEAP_CAPACITY;
}

void heap_destroy(min_heap_t* heap) {
  if (heap != NULL) {
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
  }
}

bool heap_push(min_heap_t* heap, int dist, int vertex) {
  if (heap->size == heap->capacity) {
    size_t new_capacity = heap->capacity * 2;
    heap_node_t* new_data =
        realloc(heap->data, new_capacity * sizeof(heap_node_t));
    if (new_data == NULL) {
      return false;
    }
    heap->data = new_data;
    heap->capacity = new_capacity;
  }

  heap->data[heap->size].dist = dist;
  heap->data[heap->size].vertex = vertex;
  size_t index = heap->size;
  heap->size++;

  // shift_up
  // Для min-heap меняем местами, если родитель БОЛЬШЕ текущего
  while (index > 0) {
    size_t p = parent_idx(index);
    if (heap->data[p].dist > heap->data[index].dist) {
      heap_node_t temp = heap->data[p];
      heap->data[p] = heap->data[index];
      heap->data[index] = temp;
      index = p;
    } else {
      break;
    }
  }
  return true;
}

void heap_pop(min_heap_t* heap) {
  if (heap->size == 0) {
    return;
  }

  heap->data[0] = heap->data[heap->size - 1];
  heap->size--;

  if (heap->size > 0) {
    // shift_down
    size_t index = 0;
    while (1) {
      size_t target = index;
      size_t l = left_child_idx(index);
      size_t r = right_child_idx(index);

      if (l < heap->size && heap->data[target].dist > heap->data[l].dist) {
        target = l;
      }
      if (r < heap->size && heap->data[target].dist > heap->data[r].dist) {
        target = r;
      }

      if (index == target) {
        break;
      }

      heap_node_t temp = heap->data[index];
      heap->data[index] = heap->data[target];
      heap->data[target] = temp;
      index = target;
    }
  }
}

heap_node_t heap_top(const min_heap_t* heap) { return heap->data[0]; }

bool heap_is_empty(const min_heap_t* heap) { return heap->size == 0; }