CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -Wpedantic

LIB_NAME := my_queue.a
SRC := my_queue.c
OBJ := $(SRC:.c=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

leaks: $(LIB_NAME) main.c
	$(CC) $(CFLAGS) main.c $(LIB_NAME) -o queue
	valgrind --tool=memcheck --leak-check=full ./queue

clean:
	rm -rf *.o $(LIB_NAME) queue

.PHONY: all leaks clean