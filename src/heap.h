#ifndef HEAP_H
#define HEAP_H

#define INITIAL_HEAP_CAPACITY 16

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int dist;
  int vertex;
} heap_node_t;

typedef struct {
  heap_node_t* data;
  size_t size;
  size_t capacity;
} min_heap_t;

static inline size_t parent_idx(size_t i) { return (i - 1) / 2; }

static inline size_t left_child_idx(size_t i) { return 2 * i + 1; }

static inline size_t right_child_idx(size_t i) { return 2 * i + 2; }

void heap_init(min_heap_t* heap);

void heap_destroy(min_heap_t* heap);

bool heap_push(min_heap_t* heap, int dist, int vertex);

void heap_pop(min_heap_t* heap);

heap_node_t heap_top(const min_heap_t* heap);

bool heap_is_empty(const min_heap_t* heap);

#endif