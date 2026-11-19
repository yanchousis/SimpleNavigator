#ifndef MY_QUEUE_H_
#define MY_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct my_queue {
  int size;
  int capacity;
  unsigned* arr;
} my_queue;

my_queue* queue();
void queue_destroy(my_queue**);

void queue_push(my_queue*, unsigned);
unsigned queue_pop(my_queue*);
unsigned queue_front(my_queue*);
unsigned queue_back(my_queue*);

void queue_allocate(my_queue*);

void queue_print(my_queue*, char*);

#endif