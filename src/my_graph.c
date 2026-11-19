#include "my_graph.h"

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int** allocate_matrix(int n) {
  int** matrix = calloc((size_t)n, sizeof(int*));
  if (matrix == NULL) {
    return NULL;
  }
  for (int i = 0; i < n; ++i) {
    matrix[i] = calloc((size_t)n, sizeof(int));
    if (matrix[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free(matrix[j]);
      }
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}

void free_matrix(int** matrix, int n) {
  if (matrix == NULL) {
    return;
  }
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

double** allocate_matrix_double(int n) {
  double** matrix = calloc((size_t)n, sizeof(double*));
  if (matrix == NULL) {
    return NULL;
  }
  for (int i = 0; i < n; ++i) {
    matrix[i] = calloc((size_t)n, sizeof(double));
    if (matrix[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free(matrix[j]);
      }
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}

void free_matrix_double(double** matrix, int n) {
  if (matrix == NULL) {
    return;
  }
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

static bool check_symmetry(const int** matrix, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (matrix[i][j] != matrix[j][i]) {
        return false;
      }
    }
  }
  return true;
}

static bool valid_vertex(const my_graph* graph, int v) {
  return graph != NULL && v >= 0 && v < graph->vertex_count;
}

my_graph* my_graph_create(int vertex_count) {
  if (vertex_count <= 0) {
    return NULL;
  }
  my_graph* graph = calloc(1, sizeof(my_graph));
  if (graph == NULL) {
    return NULL;
  }
  graph->vertex_count = vertex_count;
  graph->matrix = allocate_matrix(vertex_count);
  if (graph->matrix == NULL) {
    free(graph);
    return NULL;
  }
  graph->is_directed = false;
  return graph;
}

void my_graph_destroy(my_graph* graph) {
  if (graph == NULL) {
    return;
  }
  free_matrix(graph->matrix, graph->vertex_count);
  free(graph);
}

my_graph* my_graph_load_from_file(const char* filename) {
  if (filename == NULL) {
    return NULL;
  }
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    return NULL;
  }

  int vertex_count = 0;
  if (fscanf(fp, "%d", &vertex_count) != 1 || vertex_count <= 0) {
    fclose(fp);
    return NULL;
  }

  my_graph* graph = my_graph_create(vertex_count);
  if (graph == NULL) {
    fclose(fp);
    return NULL;
  }

  for (int i = 0; i < vertex_count; ++i) {
    for (int j = 0; j < vertex_count; ++j) {
      if (fscanf(fp, "%d", &graph->matrix[i][j]) != 1) {
        my_graph_destroy(graph);
        fclose(fp);
        return NULL;
      }
    }
  }
  fclose(fp);

  graph->is_directed =
      !check_symmetry((const int**)graph->matrix, vertex_count);
  return graph;
}

int my_graph_export_to_dot(const my_graph* graph, const char* filename) {
  if (graph == NULL || filename == NULL) {
    return -1;
  }
  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    return -1;
  }

  const char* connector = graph->is_directed ? "->" : "--";
  const char* header = graph->is_directed ? "digraph" : "graph";

  if (fprintf(fp, "%s {\n", header) < 0) {
    fclose(fp);
    return -1;
  }

  for (int i = 0; i < graph->vertex_count; ++i) {
    int j_start = graph->is_directed ? 0 : i;
    for (int j = j_start; j < graph->vertex_count; ++j) {
      if (graph->matrix[i][j] != 0) {
        if (graph->matrix[i][j] == 1) {
          if (fprintf(fp, "  %d %s %d;\n", i + 1, connector, j + 1) < 0) {
            fclose(fp);
            return -1;
          }
        } else {
          if (fprintf(fp, "  %d %s %d [label=%d];\n", i + 1, connector, j + 1,
                      graph->matrix[i][j]) < 0) {
            fclose(fp);
            return -1;
          }
        }
      }
    }
  }

  if (fprintf(fp, "}\n") < 0) {
    fclose(fp);
    return -1;
  }

  fclose(fp);
  return 0;
}

int my_graph_add_edge(my_graph* graph, int from, int to, int weight) {
  if (!valid_vertex(graph, from) || !valid_vertex(graph, to) || weight == 0) {
    return -1;
  }
  graph->matrix[from][to] = weight;
  if (!graph->is_directed) {
    graph->matrix[to][from] = weight;
  }
  return 0;
}

int my_graph_remove_edge(my_graph* graph, int from, int to) {
  if (!valid_vertex(graph, from) || !valid_vertex(graph, to)) {
    return -1;
  }
  graph->matrix[from][to] = 0;
  if (!graph->is_directed) {
    graph->matrix[to][from] = 0;
  }
  return 0;
}

bool my_graph_has_edge(const my_graph* graph, int from, int to) {
  if (!valid_vertex(graph, from) || !valid_vertex(graph, to)) {
    return false;
  }
  return graph->matrix[from][to] != 0;
}

int my_graph_get_weight(const my_graph* graph, int from, int to) {
  if (!valid_vertex(graph, from) || !valid_vertex(graph, to)) {
    return 0;
  }
  return graph->matrix[from][to];
}

int my_graph_get_vertex_count(const my_graph* graph) {
  return graph == NULL ? 0 : graph->vertex_count;
}

bool my_graph_is_directed(const my_graph* graph) {
  return graph == NULL ? false : graph->is_directed;
}
