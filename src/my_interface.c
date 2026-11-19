#include "my_interface.h"

void print_array(unsigned* array, int count) {
  if (!array) {
    printf("Error array for input\n");
    return;
  }

  printf("\tResult path: ");
  for (int i = 0; i < count; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

void print_array_2d(int** array, int count) {
  if (!array || !(*array)) {
    printf("Error array for input\n");
    return;
  }

  printf("\t Result path:\n");

  for (int i = 0; i < count; ++i) {
    printf("\t");

    for (int j = 0; j < count; ++j) {
      printf("%d ", array[i][j]);
    }

    printf("\n");
  }
}

void destroy_array_2d(int** array, int count) {
  if (array) {
    if ((*array)) {
      for (int i = 0; i < count; ++i) {
        free(array[i]);
      }
    }

    free(array);
  }
}

void print_start_text() {
  printf("\nSelect method:\n");
  printf("\t1. Deapth first search\n");
  printf("\t2. Breath first search\n");
  printf("\t3. Shortest path between two vertex - Dijkstra's algorithm\n");
  printf(
      "\t4. Shortest path between all pairs of vertices in a graph - "
      "Floyd-Warshall algorithm\n");
  printf("\t5. The minimal spanning tree in a graph - Prim's algorithm\n");
  printf(
      "\t6. Solving the traveling salesman's problem - The ant colony "
      "algorithm \n");
  printf("\t\t");
}

char* get_file_path() {
  char* file_path = calloc(128, sizeof(char));

  if (file_path) {
    printf("Input file name: ");

    if (fgets(file_path, 128, stdin)) {
      file_path[strcspn(file_path, "\n")] = 0;
      printf("File path: %s", file_path);
    }
    printf("\n");
  } else {
    printf("Error with memory\n");
  }

  return file_path;
}

void do_dfs(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  printf("\n\tSelect the starting vertex lower %d: ", g->vertex_count);

  int start_vertex = 0;
  scanf("%d", &start_vertex);

  unsigned* res_u_ptr = my_depth_first_search(g, start_vertex);
  print_array(res_u_ptr, g->vertex_count);

  free(res_u_ptr);
}

void do_bfs(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  printf("\n\tSelect the starting vertex lower %d: ", g->vertex_count);

  int start_vertex = 0;
  scanf("%d", &start_vertex);

  unsigned* res_u_ptr = my_breadth_first_search(g, start_vertex);
  print_array(res_u_ptr, g->vertex_count);

  free(res_u_ptr);
}

void do_shortest_path_between_vertices(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  printf("\n\tSelect two vertices: ");

  int vertex_1 = 0, vertex_2 = 0;
  scanf("%d %d", &vertex_1, &vertex_2);

  int res_int = get_shortest_path_between_vertices(g, vertex_1, vertex_2);

  if (res_int == -1) {
    printf("\tError find path\n");
  } else {
    printf("\tThe smallest distance between them: %d\n", res_int);
  }
}

void do_shortest_path_between_all_vertices(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  int** res_int_ptr_ptr = get_shortest_paths_between_all_vertices(g);

  if (res_int_ptr_ptr && *res_int_ptr_ptr)
    print_array_2d(res_int_ptr_ptr, g->vertex_count);

  destroy_array_2d(res_int_ptr_ptr, g->vertex_count);
}

void do_least_spanning_tree(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  my_graph* res_graph = get_least_spanning_tree(g);
  print_array_2d(res_graph->matrix, res_graph->vertex_count);

  my_graph_destroy(res_graph);
}

void do_solve_trabeling_salesman_problem(my_graph* g) {
  if (!g) {
    printf("Error graph\n");
    return;
  }

  tsm_result result_tsm = solve_traveling_salesman_problem(g);

  if (result_tsm.distance == -1) {
    printf("\tError find path\n");
  } else {
    // printf("\tPath: ");
    // print_array((unsigned*)result_tsm.vertices, result_tsm.distance);
    print_array((unsigned*)result_tsm.vertices, g->vertex_count);

    printf("\tDistance: %lf", result_tsm.distance);

    free(result_tsm.vertices);
  }
}

int main(void) {
  char* file_path = get_file_path();

  my_graph* g = my_graph_load_from_file(file_path);

  if (g) {
    print_start_text();

    int select = 0;
    scanf("%d", &select);

    switch (select) {
      case (1):
        do_dfs(g);
        break;
      case (2):
        do_bfs(g);
        break;
      case (3):
        do_shortest_path_between_vertices(g);
        break;
      case (4):
        do_shortest_path_between_all_vertices(g);
        break;
      case (5):
        do_least_spanning_tree(g);
        break;
      case (6):
        do_solve_trabeling_salesman_problem(g);
        break;
      default:
        printf("\tWrong choice\n");
        break;
    }
  } else {
    printf("Error graph from file\n");
  }

  free(file_path);
  my_graph_destroy(g);
  return 0;
}