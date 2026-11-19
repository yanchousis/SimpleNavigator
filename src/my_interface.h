#ifndef MY_INTERFACE_H_
#define MY_INTERFACE_H_

#include <stdio.h>
#include <string.h>

#include "my_graph.h"
#include "my_graph_algorithms.h"

void do_dfs(my_graph* g);
void do_bfs(my_graph* g);
void do_shortest_path_between_vertices(my_graph* g);
void do_shortest_path_between_all_vertices(my_graph* g);
void do_least_spanning_tree(my_graph* g);
void do_solve_trabeling_salesman_problem(my_graph* g);

char* get_file_path();

void print_start_text();

void print_array(unsigned* array, int count);
void print_array_2d(int** array, int count);

void destroy_array_2d(int** array, int count);

#endif