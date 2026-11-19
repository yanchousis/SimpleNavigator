#include "my_stack.h"

int main() {
  my_stack* s = stack();

  printf("empty top: %d\n", top(s));

  push(s, 1);
  push(s, 2);
  push(s, 3);
  push(s, 4);
  push(s, 5);

  print_stack(s, "push 5: ");

  printf("top: %d\n", top(s));

  printf("pop: %d\n", pop(s));
  printf("top after pop: %d\n", top(s));
  print_stack(s, "print after pop");

  printf("pop: %d\n", pop(s));
  printf("top after pop: %d\n", top(s));
  print_stack(s, "print after pop");

  printf("pop: %d\n", pop(s));
  printf("top after pop: %d\n", top(s));
  print_stack(s, "print after pop");

  printf("pop: %d\n", pop(s));
  printf("top after pop: %d\n", top(s));
  print_stack(s, "print after pop");

  printf("pop: %d\n", pop(s));
  printf("top after pop: %d\n", top(s));
  print_stack(s, "print after pop");

  destroy_stack(&s);

  return 0;
}