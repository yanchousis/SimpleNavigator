#ifndef MY_STACK_H_
#define MY_STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct my_stack {
  int size;
  int capacity;
  unsigned* arr;
} my_stack;

my_stack* stack();
void stack_destroy(my_stack**);

void stack_push(my_stack*, unsigned);
unsigned stack_pop(my_stack*);
unsigned stack_top(my_stack*);

void stack_allocate(my_stack*);

void stack_print(my_stack*, char*);

#endif