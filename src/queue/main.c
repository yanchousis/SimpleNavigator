#include "my_queue.h"

int main(void) {
  my_queue* q = queue();

  queue_push(q, 1);
  queue_push(q, 2);
  queue_push(q, 3);
  queue_push(q, 4);
  queue_push(q, 5);
  // 54321

  queue_print(q, "push 5: ");

  printf("front & back: %d, %d\n", queue_front(q), queue_back(q));

  printf("pop: %d\n", queue_pop(q));
  printf("front & back after pop: %d, %d\n", queue_front(q), queue_back(q));
  queue_print(q, "print after pop");

  printf("pop: %d\n", queue_pop(q));
  printf("front & back after pop: %d, %d\n", queue_front(q), queue_back(q));
  queue_print(q, "print after pop");

  printf("pop: %d\n", queue_pop(q));
  printf("front & back after pop: %d, %d\n", queue_front(q), queue_back(q));
  queue_print(q, "print after pop");

  printf("pop: %d\n", queue_pop(q));
  printf("front & back after pop: %d, %d\n", queue_front(q), queue_back(q));
  queue_print(q, "print after pop");

  printf("pop: %d\n", queue_pop(q));
  printf("front & back after pop: %d, %d\n", queue_front(q), queue_back(q));
  queue_print(q, "print after pop");

  queue_destroy(&q);

  return 0;
}
