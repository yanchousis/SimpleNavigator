#include "my_graph_algorithms.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned* my_depth_first_search(my_graph* graph, int start_vertex) {
  if (!graph || !graph->matrix || start_vertex > graph->vertex_count ||
      start_vertex < 1)
    return NULL;

  unsigned* result = calloc(graph->vertex_count, sizeof(unsigned));
  bool* repeat_arr = calloc(graph->vertex_count, sizeof(bool));
  my_stack* s = stack();

  stack_push(s, start_vertex);
  unsigned number_added_vertex = 0;
  result[number_added_vertex++] = stack_top(s);
  repeat_arr[stack_top(s) - 1] = true;

  for (int j = 0; j < graph->vertex_count && s->size; ++j) {
    if (graph->matrix[stack_top(s) - 1][j] && !repeat_arr[j]) {
      stack_push(s, j + 1);
      result[number_added_vertex++] = j + 1;
      repeat_arr[j] = true;
      j = -1;
    } else if (j == graph->vertex_count - 1) {
      j = stack_pop(s) - 1 - 1;
    }
  }

  free(repeat_arr);
  stack_destroy(&s);
  return result;
}

unsigned* my_breadth_first_search(my_graph* graph, int start_vertex) {
  if (!graph || !graph->matrix || start_vertex > graph->vertex_count ||
      start_vertex < 1)
    return NULL;

  unsigned* result = calloc(graph->vertex_count, sizeof(unsigned));
  bool* repeat_arr = calloc(graph->vertex_count, sizeof(bool));
  my_queue* q = queue();

  queue_push(q, start_vertex);
  unsigned number_added_vertex = 0;
  result[number_added_vertex++] = queue_back(q);
  repeat_arr[queue_back(q) - 1] = true;

  for (int j = 0; j < graph->vertex_count && q->size; ++j) {
    if (graph->matrix[queue_back(q) - 1][j] && !repeat_arr[j]) {
      queue_push(q, j + 1);
      result[number_added_vertex++] = j + 1;
      repeat_arr[j] = true;
    }

    if (j == graph->vertex_count - 1) {
      queue_pop(q);
      j = -1;
    }
  }

  free(repeat_arr);
  queue_destroy(&q);
  return result;
}

int get_shortest_path_between_vertices(const my_graph* graph, int vertex1,
                                       int vertex2) {
  if (graph == NULL || vertex1 < 0 || vertex1 >= graph->vertex_count ||
      vertex2 < 0 || vertex2 >= graph->vertex_count) {
    return -1;
  }

  if (vertex1 == vertex2) {
    return 0;
  }

  int n = graph->vertex_count;
  int* dist = malloc((size_t)n * sizeof(int));
  if (dist == NULL) {
    return -1;
  }

  for (int i = 0; i < n; i++) {
    dist[i] = INF;
  }
  dist[vertex1] = 0;

  min_heap_t pq;
  heap_init(&pq);

  if (!heap_push(&pq, 0, vertex1)) {
    free(dist);
    heap_destroy(&pq);
    return -1;
  }

  while (!heap_is_empty(&pq)) {
    heap_node_t current = heap_top(&pq);
    heap_pop(&pq);

    int d = current.dist;
    int u = current.vertex;

    if (d > dist[u]) {
      continue;
    }

    if (u == vertex2) {
      break;
    }

    for (int v = 0; v < n; v++) {
      int weight = graph->matrix[u][v];

      if (weight > 0) {
        if (dist[u] != INF && dist[u] + weight < dist[v]) {
          dist[v] = dist[u] + weight;
          heap_push(&pq, dist[v], v);
        }
      }
    }
  }

  int result = (dist[vertex2] == INF) ? -1 : dist[vertex2];

  free(dist);
  heap_destroy(&pq);

  return result;
}

static double get_safe_weight(my_graph* g, int i, int j) {
  if (i == j) return 0.0;
  double w = (double)g->matrix[i][j];
  if (w == 0.0) return INF;
  return w;
}

// Основная функция алгоритма Флойда-Уоршелла
int** get_shortest_paths_between_all_vertices(my_graph* g) {
  int n = g->vertex_count;

  int** dist = (int**)malloc(n * sizeof(int*));
  if (!dist) return NULL;

  for (int i = 0; i < n; i++) {
    dist[i] = (int*)malloc(n * sizeof(int));
    if (!dist[i]) {
      free_matrix(dist, i);
      return NULL;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        dist[i][j] = 0;
      } else {
        dist[i][j] = get_safe_weight(g,i,j);
      }
    }
  }

  // k - промежуточная вершина
  for (int k = 0; k < n; k++) {
    // i - начальная вершина
    for (int i = 0; i < n; i++) {
      // j - конечная вершина
      for (int j = 0; j < n; j++) {
        // ВАЖНО: Проверяем, что пути до k и от k существуют (не равны INF).
        // Это защищает от переполнения int при сложении INF + INF.
        if (dist[i][k] < INF && dist[k][j] < INF) {
          // Релаксация: если путь через k короче, обновляем его
          if (dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
          }
        }
      }
    }
  }

  return dist;
}

my_graph* get_least_spanning_tree(my_graph* g) {
  if (g == NULL || g->matrix == NULL || g->vertex_count <= 0) {
    my_graph* empty_graph = NULL;
    return empty_graph;
  }

  int n = g->vertex_count;

  my_graph* mst = my_graph_create(n);

  bool* in_mst =
      (bool*)calloc(n, sizeof(bool));  // Вершины, уже включенные в MST
  int* key = (int*)malloc(
      n * sizeof(int));  // Минимальный вес ребра для подключения вершины
  int* parent = (int*)malloc(n * sizeof(int));  // Предок вершины в MST

  for (int i = 0; i < n; i++) {
    key[i] = INF;
    parent[i] = -1;
  }

  key[0] = 0;

  for (int count = 0; count < n; count++) {
    // 1. Находим вершину 'u' с минимальным ключом, которая еще не в MST
    int u = -1;
    int min_key = INF;

    for (int v = 0; v < n; v++) {
      if (!in_mst[v] && key[v] < min_key) {
        min_key = key[v];
        u = v;
      }
    }

    // Если граф несвязный и мы не можем достичь оставшиеся вершины
    if (u == -1) {
      break;
    }

    // 2. Добавляем найденную вершину в MST
    in_mst[u] = true;

    // 3. Обновляем ключи и предков для смежных вершин
    for (int v = 0; v < n; v++) {
      if (g->matrix[u][v] != 0 && !in_mst[v] && g->matrix[u][v] < key[v]) {
        parent[v] = u;
        key[v] = g->matrix[u][v];
      }
    }
  }

  for (int i = 1; i < n; i++) {
    if (parent[i] != -1) {
      int u = parent[i];
      int v = i;
      int weight = key[i];

      mst->matrix[u][v] = weight;
      mst->matrix[v][u] = weight;
    }
  }

  free(in_mst);
  free(key);
  free(parent);

  return mst;
}



static int init_aco_matrices(my_graph* g, int n, double*** pheromone,
                             double*** heuristic) {
  *pheromone = allocate_matrix_double(n);
  *heuristic = allocate_matrix_double(n);
  if (!*pheromone || !*heuristic) {
    if (*pheromone) free_matrix_double(*pheromone, n);
    if (*heuristic) free_matrix_double(*heuristic, n);
    return 0;
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i != j) {
        (*pheromone)[i][j] = 1.0;
        double dist = get_safe_weight(g, i, j);
        (*heuristic)[i][j] = (dist < INF) ? (1.0 / dist) : 0.0;
      }
    }
  }
  return 1;
}

static int select_next_vertex(int current, int n, int* visited, my_graph* g,
                              double** pheromone, double** heuristic) {
  double sum_prob = 0.0;
  double* prob = calloc(n, sizeof(double));
  if (!prob) return -1;

  for (int next = 0; next < n; next++) {
    double w = get_safe_weight(g, current, next);
    if (!visited[next] && w < INF) {
      prob[next] = pow(pheromone[current][next], ACO_ALPHA) *
                   pow(heuristic[current][next], ACO_BETA);
      sum_prob += prob[next];
    }
  }

  if (sum_prob == 0.0) {
    free(prob);
    return -1;
  }

  double r = ((double)rand() / RAND_MAX) * sum_prob;
  double cumulative = 0.0;
  int next_vertex = -1;

  for (int next = 0; next < n; next++) {
    if (prob[next] > 0) {
      cumulative += prob[next];
      if (cumulative >= r) {
        next_vertex = next;
        break;
      }
    }
  }

  if (next_vertex == -1) {
    for (int next = 0; next < n; next++) {
      if (prob[next] > 0) {
        next_vertex = next;
        break;
      }
    }
  }

  free(prob);
  return next_vertex;
}

static int* build_ant_tour(my_graph* g, int n, double** pheromone,
                           double** heuristic, double* tour_dist) {
  int* tour = malloc(n * sizeof(int));
  int* visited = calloc(n, sizeof(int));
  if (!tour || !visited) {
    free(tour);
    free(visited);
    return NULL;
  }

  int current = rand() % n;
  tour[0] = current;
  visited[current] = 1;
  *tour_dist = 0.0;

  for (int step = 1; step < n; step++) {
    int next_vertex =
        select_next_vertex(current, n, visited, g, pheromone, heuristic);
    if (next_vertex == -1) {
      free(tour);
      free(visited);
      return NULL;
    }

    *tour_dist += get_safe_weight(g, current, next_vertex);
    tour[step] = next_vertex;
    visited[next_vertex] = 1;
    current = next_vertex;
  }

  double return_dist = get_safe_weight(g, current, tour[0]);
  if (return_dist >= INF) {
    free(tour);
    free(visited);
    return NULL;
  }
  *tour_dist += return_dist;

  free(visited);
  return tour;
}

static void deposit_pheromones(double** pheromone, int* tour, int n,
                               double tour_dist) {
  double deposit = ACO_Q / tour_dist;
  for (int i = 0; i < n; i++) {
    int u = tour[i];
    int v = tour[(i + 1) % n];
    pheromone[u][v] += deposit;
    pheromone[v][u] += deposit;
  }
}

static void evaporate_pheromones(double** pheromone, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i != j) {
        pheromone[i][j] *= (1.0 - ACO_RHO);
        if (pheromone[i][j] < 0.01) pheromone[i][j] = 0.01;
      }
    }
  }
}

tsm_result solve_traveling_salesman_problem(my_graph* g) {
  tsm_result result = {NULL, -1.0};
  if (!g || g->vertex_count <= 0 || !g->matrix) return result;

  int n = g->vertex_count;
  if (n == 1) {
    result.vertices = malloc(sizeof(int));
    if (result.vertices) {
      result.vertices[0] = 0;
      result.distance = 0.0;
    }
    return result;
  }

  double **pheromone = NULL, **heuristic = NULL;
  if (!init_aco_matrices(g, n, &pheromone, &heuristic)) return result;

  int* best_tour = malloc(n * sizeof(int));
  double best_distance = INF;
  srand((unsigned int)time(NULL));

  for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
    for (int ant = 0; ant < NUM_ANTS; ant++) {
      double tour_dist = 0.0;
      int* tour = build_ant_tour(g, n, pheromone, heuristic, &tour_dist);
      if (tour) {
        if (tour_dist < best_distance) {
          best_distance = tour_dist;
          memcpy(best_tour, tour, n * sizeof(int));
        }
        deposit_pheromones(pheromone, tour, n, tour_dist);
        free(tour);
      }
    }
    evaporate_pheromones(pheromone, n);
  }

  free_matrix_double(pheromone, n);
  free_matrix_double(heuristic, n);

  if (best_distance < INF) {
    result.vertices = best_tour;
    result.distance = best_distance;
  } else {
    free(best_tour);
  }

  return result;
}
