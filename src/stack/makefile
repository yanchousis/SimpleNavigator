CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -Wpedantic

LIB_NAME := my_stack.a
SRC := my_stack.c
OBJ := $(SRC:.c=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

leaks: $(LIB_NAME) main.c
	$(CC) $(CFLAGS) main.c $(LIB_NAME) -o stack
	valgrind --tool=memcheck --leak-check=full ./stack

clean:
	rm -rf *.o $(LIB_NAME) stack

.PHONY: all leaks clean