CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=201710L
LCOV_FLAGS = -fprofile-arcs -ftest-coverage

BUILD_DIR = build

LIB_SRC_STACK := stack/my_stack.c
LIB_OBJ_STACK := $(BUILD_DIR)/my_stack.o
LIB_NAME_STACK := $(BUILD_DIR)/my_stack.a

LIB_SRC_QUEUE := queue/my_queue.c
LIB_OBJ_QUEUE := $(BUILD_DIR)/my_queue.o
LIB_NAME_QUEUE := $(BUILD_DIR)/my_queue.a

LIB_SRC_1 = my_graph.c
LIB_OBJ_1 = $(BUILD_DIR)/$(LIB_SRC_1:.c=.o)
LIB_NAME_1 = $(BUILD_DIR)/my_graph.a

LIB_SRC_2 = my_graph_algorithms.c heap.c
LIB_OBJ_2 = $(addprefix $(BUILD_DIR)/, $(LIB_SRC_2:.c=.o))
# LIB_OBJ_2 = $(BUILD_DIR)/$(LIB_SRC_2:.c=.o)
LIB_NAME_2 = $(BUILD_DIR)/my_graph_algorithms.a

TEST_SRC_1 = tests/my_graph_test.c
TEST_SRC_2 = tests/my_graph_algorithms_test.c

TEST_BIN_1 = $(BUILD_DIR)/test_my_graph
TEST_BIN_2 = $(BUILD_DIR)/test_my_graph_algorithms

SRC_INTERFACE = my_interface.c

SRC_INTERFACE = my_interface.c

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    CHECK_LIBS = -lcheck
else
    CHECK_LIBS = -lcheck -lsubunit -lm -lpthread
endif

.PHONY: all clean my_graph lcov_report my_graph_algorithms test_graph test_graph_algorithms

all: my_graph my_graph_algorithms my_interface

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_NAME_STACK): $(LIB_OBJ_STACK)
	ar rcs $@ $^
	ranlib $@ 2>/dev/null || true

$(BUILD_DIR)/%.o: stack/%.c stack/my_stack.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME_QUEUE): $(LIB_OBJ_QUEUE)
	ar rcs $@ $^
	ranlib $@ 2>/dev/null || true

$(BUILD_DIR)/%.o: queue/%.c queue/my_queue.h
	$(CC) $(CFLAGS) -c $< -o $@


my_graph: $(LIB_NAME_1)

$(LIB_NAME_1): $(LIB_OBJ_1)
	ar rcs $@ $^
	ranlib $@ 2>/dev/null || true

$(BUILD_DIR)/%.o: %.c my_graph | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

my_graph_algorithms: $(LIB_NAME_STACK) $(LIB_NAME_QUEUE) $(LIB_NAME_2)

$(LIB_NAME_2): $(LIB_OBJ_2) $(LIB_OBJ_STACK) $(LIB_OBJ_QUEUE)
	ar rcs $@ $^
	ranlib $@ 2>/dev/null || true

$(BUILD_DIR)/%.o: %.c my_graph_algorithms.h heap.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

my_interface: $(LIB_NAME_STACK) $(LIB_NAME_QUEUE) $(LIB_NAME_1) $(LIB_NAME_2)
	$(CC) $(CFLAGS)  $(SRC_INTERFACE) $(LIB_NAME_1) $(LIB_NAME_2) $(LIB_NAME_STACK) $(LIB_NAME_QUEUE) -o $@ -lm

test: test_graph test_graph_algorithms

test_graph: $(TEST_BIN_1)
	./$(TEST_BIN_1)

$(TEST_BIN_1): $(TEST_SRC_1) $(LIB_NAME_1) $(LIB_NAME_2) heap.h my_graph.h my_graph_algorithms.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC_1) $(LIB_NAME_1) $(LIB_NAME_2) -o $@ $(CHECK_LIBS)

test_graph_algorithms: $(TEST_BIN_2)
	./$(TEST_BIN_2)

$(TEST_BIN_2): $(TEST_SRC_2) $(LIB_NAME_STACK) $(LIB_NAME_QUEUE) $(LIB_NAME_1) $(LIB_NAME_2) stack/my_stack.h queue/my_queue.h my_graph.h heap.h my_graph_algorithms.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC_2) $(LIB_NAME_STACK) $(LIB_NAME_QUEUE) $(LIB_NAME_1) $(LIB_NAME_2) -o $@ $(CHECK_LIBS)

lcov_report: CFLAGS += $(LCOV_FLAGS)
lcov_report: clean $(TEST_BIN_1) $(TEST_BIN_2)
	./$(TEST_BIN_1)
	./$(TEST_BIN_2)
	lcov --capture --directory $(BUILD_DIR) --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	@echo "Open coverage_report/index.html"

clean:
	rm -rf $(BUILD_DIR)
	rm -f coverage.info
	rm -rf coverage_report
	rm -f my_interface