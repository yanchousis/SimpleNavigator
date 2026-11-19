#include "../my_graph_algorithms.h"

#include <check.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "../my_graph.h"
#include "../my_graph_algorithms.h"

#define EPS 1e-6
#define TEST_FILE "test.txt"

//  ТЕСТЫ: get_shortest_path_between_vertices (Dijkstra / BFS)

int get_shortest_path_between_vertices(const my_graph* graph, int vertex1,
                                       int vertex2);

START_TEST(test_null_graph) {
  int result = get_shortest_path_between_vertices(NULL, 0, 1);
  ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_negative_vertex1) {
  my_graph* g = my_graph_create(3);
  int result = get_shortest_path_between_vertices(g, -1, 1);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_negative_vertex2) {
  my_graph* g = my_graph_create(3);
  int result = get_shortest_path_between_vertices(g, 0, -5);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_vertex1_out_of_bounds) {
  my_graph* g = my_graph_create(3);
  int result = get_shortest_path_between_vertices(g, 3, 1);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_vertex2_out_of_bounds) {
  my_graph* g = my_graph_create(3);
  int result = get_shortest_path_between_vertices(g, 0, 10);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_same_vertex) {
  my_graph* g = my_graph_create(3);
  int result = get_shortest_path_between_vertices(g, 1, 1);
  ck_assert_int_eq(result, 0);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_single_vertex_graph) {
  my_graph* g = my_graph_create(1);
  int result = get_shortest_path_between_vertices(g, 0, 0);
  ck_assert_int_eq(result, 0);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_no_edge_between_vertices) {
  my_graph* g = my_graph_create(2);
  int result = get_shortest_path_between_vertices(g, 0, 1);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_simple_undirected_path) {
  my_graph* g = my_graph_create(2);
  g->is_directed = false;
  my_graph_add_edge(g, 0, 1, 5);
  my_graph_add_edge(g, 1, 0, 5);

  int result = get_shortest_path_between_vertices(g, 0, 1);
  ck_assert_int_eq(result, 5);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_shortest_of_two_paths_undirected) {
  my_graph* g = my_graph_create(4);
  g->is_directed = false;

  my_graph_add_edge(g, 0, 1, 1);
  my_graph_add_edge(g, 1, 0, 1);
  my_graph_add_edge(g, 1, 2, 1);
  my_graph_add_edge(g, 2, 1, 1);
  my_graph_add_edge(g, 0, 3, 10);
  my_graph_add_edge(g, 3, 0, 10);
  my_graph_add_edge(g, 3, 2, 10);
  my_graph_add_edge(g, 2, 3, 10);

  // Путь 1: 0 -> 1 -> 2 (вес 1 + 1 = 2)
  my_graph_add_edge(g, 0, 1, 1);
  my_graph_add_edge(g, 1, 0, 1);
  my_graph_add_edge(g, 1, 2, 1);
  my_graph_add_edge(g, 2, 1, 1);

  // Путь 2: 0 -> 3 -> 2 (вес 10 + 10 = 20)
  my_graph_add_edge(g, 0, 3, 10);
  my_graph_add_edge(g, 3, 0, 10);
  my_graph_add_edge(g, 3, 2, 10);
  my_graph_add_edge(g, 2, 3, 10);

  int result = get_shortest_path_between_vertices(g, 0, 2);
  ck_assert_int_eq(result, 2);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_complex_directed_graph) {
  my_graph* g = my_graph_create(6);
  g->is_directed = true;

  my_graph_add_edge(g, 0, 1, 4);
  my_graph_add_edge(g, 0, 3, 2);
  my_graph_add_edge(g, 1, 2, 8);
  my_graph_add_edge(g, 1, 4, 3);
  my_graph_add_edge(g, 2, 5, 1);
  my_graph_add_edge(g, 3, 4, 1);
  my_graph_add_edge(g, 4, 5, 6);

  int result = get_shortest_path_between_vertices(g, 0, 5);
  ck_assert_int_eq(result, 9);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_directed_no_reverse_path) {
  my_graph* g = my_graph_create(2);
  g->is_directed = true;
  my_graph_add_edge(g, 0, 1, 5);

  int forward = get_shortest_path_between_vertices(g, 0, 1);
  ck_assert_int_eq(forward, 5);

  int backward = get_shortest_path_between_vertices(g, 1, 0);
  ck_assert_int_eq(backward, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_disconnected_graph) {
  my_graph* g = my_graph_create(4);
  g->is_directed = false;

  my_graph_add_edge(g, 0, 1, 5);
  my_graph_add_edge(g, 1, 0, 5);
  my_graph_add_edge(g, 2, 3, 3);
  my_graph_add_edge(g, 3, 2, 3);

  int result = get_shortest_path_between_vertices(g, 0, 3);
  ck_assert_int_eq(result, -1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_graph_with_cycle) {
  my_graph* g = my_graph_create(4);
  g->is_directed = true;

  my_graph_add_edge(g, 0, 1, 1);
  my_graph_add_edge(g, 1, 2, 1);
  my_graph_add_edge(g, 2, 3, 1);
  my_graph_add_edge(g, 3, 1, 1);

  int result = get_shortest_path_between_vertices(g, 0, 3);
  ck_assert_int_eq(result, 3);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_large_weights_no_overflow) {
  my_graph* g = my_graph_create(3);
  g->is_directed = true;

  my_graph_add_edge(g, 0, 1, 1000000);
  my_graph_add_edge(g, 1, 2, 1000000);

  // INF = 1000000000. Сумма 2000000 далеко до переполнения int и < INF
  my_graph_add_edge(g, 0, 1, 1000000);
  my_graph_add_edge(g, 1, 2, 1000000);

  int result = get_shortest_path_between_vertices(g, 0, 2);
  ck_assert_int_eq(result, 2000000);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_full_graph_shortcut) {
  my_graph* g = my_graph_create(4);
  g->is_directed = true;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i != j) {
        my_graph_add_edge(g, i, j, 10);
      }
    }
  }
  my_graph_add_edge(g, 0, 3, 100);

  int result = get_shortest_path_between_vertices(g, 0, 3);
  ck_assert_int_eq(result, 20);
  my_graph_destroy(g);
}
END_TEST

//   ТЕСТЫ: get_shortest_paths_between_all_vertices (Floyd-Warshall)

int** get_shortest_paths_between_all_vertices(my_graph* g);
void free_matrix(int** matrix, int size);

static my_graph* create_fw_graph(int n) {
  my_graph* g = my_graph_create(n);
  if (!g) return NULL;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i != j) {
        g->matrix[i][j] = INF;
      }
    }
  }

  return g;
}

static void add_fw_edge(my_graph* g, int from, int to, int weight) {
  g->matrix[from][to] = weight;
  if (!g->is_directed) {
    g->matrix[to][from] = weight;
  }
}

START_TEST(test_fw_single_vertex) {
  my_graph* g = create_fw_graph(1);
  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][0], 0);

  free_matrix(result, 1);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_two_vertices_no_edge) {
  my_graph* g = create_fw_graph(2);
  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][0], 0);
  ck_assert_int_eq(result[1][1], 0);
  ck_assert_int_eq(result[0][1], INF);
  ck_assert_int_eq(result[1][0], INF);

  free_matrix(result, 2);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_simple_triangle_relaxation) {
  my_graph* g = create_fw_graph(3);
  g->is_directed = true;

  add_fw_edge(g, 0, 1, 10);
  add_fw_edge(g, 1, 2, 10);
  add_fw_edge(g, 0, 2, 100);

  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][2], 20);
  ck_assert_int_eq(result[0][1], 10);
  ck_assert_int_eq(result[1][2], 10);

  free_matrix(result, 3);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_disconnected_components) {
  my_graph* g = create_fw_graph(4);
  g->is_directed = false;

  add_fw_edge(g, 0, 1, 7);
  add_fw_edge(g, 2, 3, 3);

  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][3], INF);
  ck_assert_int_eq(result[1][2], INF);
  ck_assert_int_eq(result[0][1], 7);
  ck_assert_int_eq(result[2][3], 3);

  free_matrix(result, 4);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_multiple_intermediates) {
  my_graph* g = create_fw_graph(5);
  g->is_directed = true;

  for (int i = 0; i < 4; i++) {
    add_fw_edge(g, i, i + 1, 1);
  }

  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][4], 4);
  ck_assert_int_eq(result[1][3], 2);
  ck_assert_int_eq(result[4][0], INF);

  free_matrix(result, 5);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_protection_against_overflow) {
  my_graph* g = create_fw_graph(3);
  g->is_directed = true;

  add_fw_edge(g, 0, 1, 50);

  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][2], INF);
  ck_assert_int_eq(result[1][2], INF);

  free_matrix(result, 3);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_fw_cycle_does_not_break) {
  my_graph* g = create_fw_graph(3);
  g->is_directed = true;

  add_fw_edge(g, 0, 1, 1);
  add_fw_edge(g, 1, 2, 1);
  add_fw_edge(g, 2, 0, 1);

  int** result = get_shortest_paths_between_all_vertices(g);

  ck_assert_ptr_nonnull(result);
  ck_assert_int_eq(result[0][0], 0);
  ck_assert_int_eq(result[1][1], 0);
  ck_assert_int_eq(result[2][2], 0);
  ck_assert_int_eq(result[0][2], 2);
  ck_assert_int_eq(result[2][1], 2);

  free_matrix(result, 3);
  my_graph_destroy(g);
}
END_TEST

//  ТЕСТЫ: get_least_spanning_tree (Алгоритм Прима)

// 1. Некорректные входные данные: NULL
START_TEST(test_mst_null_graph) {
  my_graph* res = get_least_spanning_tree(NULL);
  ck_assert_ptr_eq(res, NULL);
}
END_TEST

// 2. Некорректные входные данные: 0 вершин
START_TEST(test_mst_empty_graph) {
  my_graph* g = my_graph_create(0);
  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_eq(res, NULL);

  my_graph_destroy(g);
}
END_TEST

// 3. Крайний случай: 1 вершина
START_TEST(test_mst_single_vertex) {
  my_graph* g = my_graph_create(1);
  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);
  ck_assert_int_eq(res->vertex_count, 1);
  ck_assert_int_eq(res->matrix[0][0], 0);  // Нет петель

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

// 4. Простой связный граф: 2 вершины
START_TEST(test_mst_two_vertices) {
  my_graph* g = my_graph_create(2);
  g->matrix[0][1] = 42;
  g->matrix[1][0] = 42;

  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);
  ck_assert_int_eq(res->vertex_count, 2);
  ck_assert_int_eq(res->matrix[0][1], 42);
  ck_assert_int_eq(res->matrix[1][0], 42);

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

// 5. Классический пример: треугольник (3 вершины)
// Ребра: (0,1)=10, (1,2)=20, (0,2)=30. MST должно выбрать (0,1) и (1,2).
START_TEST(test_mst_simple_triangle) {
  my_graph* g = my_graph_create(3);
  g->matrix[0][1] = g->matrix[1][0] = 10;
  g->matrix[1][2] = g->matrix[2][1] = 20;
  g->matrix[0][2] = g->matrix[2][0] =
      30;  // Самое тяжелое ребро, не должно попасть в MST

  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);
  ck_assert_int_eq(res->vertex_count, 3);

  // Проверяем, что в MST попали только нужные ребра
  ck_assert_int_eq(res->matrix[0][1], 10);
  ck_assert_int_eq(res->matrix[1][0], 10);
  ck_assert_int_eq(res->matrix[1][2], 20);
  ck_assert_int_eq(res->matrix[2][1], 20);

  // Ребро (0,2) не должно быть в MST
  ck_assert_int_eq(res->matrix[0][2], 0);
  ck_assert_int_eq(res->matrix[2][0], 0);

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

// 6. Граф с "дорогим" прямым путем и "дешевым" обходным
START_TEST(test_mst_linear_with_shortcut) {
  my_graph* g = my_graph_create(4);
  // Дешевый путь: 0 -1-> 1 -2-> 2 -3-> 3 (сумма = 6)
  g->matrix[0][1] = g->matrix[1][0] = 1;
  g->matrix[1][2] = g->matrix[2][1] = 2;
  g->matrix[2][3] = g->matrix[3][2] = 3;
  // Дорогой прямой путь: 0 -10-> 3
  g->matrix[0][3] = g->matrix[3][0] = 10;

  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);

  ck_assert_int_eq(res->matrix[0][1], 1);
  ck_assert_int_eq(res->matrix[1][2], 2);
  ck_assert_int_eq(res->matrix[2][3], 3);

  // Дорогое ребро не должно быть использовано
  ck_assert_int_eq(res->matrix[0][3], 0);

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

// 7. Несвязный граф (проверка поведения при u == -1)
// Алгоритм начинает с вершины 0. Он должен построить MST только для компоненты
// {0, 1}. Вершины {2, 3} должны остаться изолированными в результирующем графе.
START_TEST(test_mst_disconnected_graph) {
  my_graph* g = my_graph_create(4);

  // Компонента 1: вершины 0 и 1
  g->matrix[0][1] = g->matrix[1][0] = 5;

  // Компонента 2: вершины 2 и 3 (недостижима из 0)
  g->matrix[2][3] = g->matrix[3][2] = 10;

  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);

  // Компонента, содержащая 0, должна быть соединена
  ck_assert_int_eq(res->matrix[0][1], 5);
  ck_assert_int_eq(res->matrix[1][0], 5);

  // Компонента {2, 3} НЕ должна быть соединена в результате,
  // так как алгоритм Прима не может до нее добраться из вершины 0.
  ck_assert_int_eq(res->matrix[2][3], 0);
  ck_assert_int_eq(res->matrix[3][2], 0);

  // Между компонентами связей быть не должно
  ck_assert_int_eq(res->matrix[0][2], 0);
  ck_assert_int_eq(res->matrix[1][3], 0);

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

// 8. Полносвязный граф (проверка корректности работы на плотных графах)
START_TEST(test_mst_fully_connected) {
  my_graph* g = my_graph_create(4);

  // Задаем веса так, чтобы MST было очевидно: звездой из центра 0
  g->matrix[0][1] = g->matrix[1][0] = 1;
  g->matrix[0][2] = g->matrix[2][0] = 1;
  g->matrix[0][3] = g->matrix[3][0] = 1;

  // Все остальные ребра тяжелые
  g->matrix[1][2] = g->matrix[2][1] = 100;
  g->matrix[1][3] = g->matrix[3][1] = 100;
  g->matrix[2][3] = g->matrix[3][2] = 100;

  my_graph* res = get_least_spanning_tree(g);

  ck_assert_ptr_nonnull(res);

  // Должны быть выбраны только легкие ребра от вершины 0
  ck_assert_int_eq(res->matrix[0][1], 1);
  ck_assert_int_eq(res->matrix[0][2], 1);
  ck_assert_int_eq(res->matrix[0][3], 1);

  // Тяжелые ребра не должны попасть в MST
  ck_assert_int_eq(res->matrix[1][2], 0);
  ck_assert_int_eq(res->matrix[2][3], 0);

  my_graph_destroy(g);
  my_graph_destroy(res);
}
END_TEST

//  * ТЕСТЫ: solve_traveling_salesman_problem (Ant Colony)

START_TEST(test_tsp_null_input) {
  tsm_result res = solve_traveling_salesman_problem(NULL);
  ck_assert_ptr_eq(res.vertices, NULL);
  ck_assert_double_eq(res.distance, -1.0);
}
END_TEST

START_TEST(test_tsp_empty_graph) {
  my_graph* g = my_graph_create(0);
  tsm_result res = solve_traveling_salesman_problem(g);

  ck_assert_ptr_eq(res.vertices, NULL);
  ck_assert_double_eq(res.distance, -1.0);

  my_graph_destroy(g);
}
END_TEST

START_TEST(test_tsp_one_vertex) {
  my_graph* g = my_graph_create(1);
  tsm_result res = solve_traveling_salesman_problem(g);

  ck_assert_ptr_nonnull(res.vertices);
  ck_assert_int_eq(res.vertices[0], 0);
  ck_assert_double_eq_tol(res.distance, 0.0, EPS);

  free(res.vertices);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_tsp_two_vertices) {
  my_graph* g = my_graph_create(2);
  g->matrix[0][1] = 15;
  g->matrix[1][0] = 15;

  tsm_result res = solve_traveling_salesman_problem(g);

  ck_assert_ptr_nonnull(res.vertices);
  ck_assert((res.vertices[0] == 0 && res.vertices[1] == 1) ||
            (res.vertices[0] == 1 && res.vertices[1] == 0));
  ck_assert_double_eq_tol(res.distance, 30.0, EPS);

  free(res.vertices);
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_tsp_disconnected_graph) {
  my_graph* g = my_graph_create(3);
  g->matrix[0][1] = 10;
  g->matrix[1][0] = 10;
  // Вершина 2 изолирована (вес 0)

  tsm_result res = solve_traveling_salesman_problem(g);

  ck_assert_ptr_eq(res.vertices, NULL);
  ck_assert_double_eq(res.distance, -1.0);

  my_graph_destroy(g);
}
END_TEST

START_TEST(test_tsp_known_optimal_tour) {
  my_graph* g = my_graph_create(4);

  int weights[4][4] = {
      {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      g->matrix[i][j] = weights[i][j];
    }
  }

  tsm_result res = solve_traveling_salesman_problem(g);

  ck_assert_ptr_nonnull(res.vertices);

  int visited[4] = {0};
  for (int i = 0; i < 4; i++) {
    ck_assert(res.vertices[i] >= 0 && res.vertices[i] < 4);
    visited[res.vertices[i]]++;
  }
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(visited[i], 1);
  }

  ck_assert_double_ge(res.distance, 80.0 - EPS);
  ck_assert_double_le(res.distance, 85.0);

  free(res.vertices);
  my_graph_destroy(g);
}
END_TEST

//  ТЕСТЫ: DFS

static void write_test_file(const char* path, const char* content) {
  FILE* fp = fopen(path, "w");
  ck_assert_ptr_nonnull(fp);
  fputs(content, fp);
  fclose(fp);
}

START_TEST(test_null_graph_dfs) {
  ck_assert_ptr_null(my_depth_first_search(NULL, 2));
}
END_TEST

START_TEST(test_null_graph_matrix_dfs) {
  my_graph* g = malloc(sizeof(my_graph));
  g->matrix = NULL;
  g->is_directed = 0;
  g->vertex_count = 1;

  ck_assert_ptr_null(my_depth_first_search(NULL, 2));

  free(g);
}
END_TEST

START_TEST(test_vertex_more_dfs) {
  my_graph* g = malloc(sizeof(my_graph));
  g->matrix = NULL;
  g->is_directed = 0;
  g->vertex_count = 1;

  ck_assert_ptr_null(my_depth_first_search(NULL, 2));

  free(g);
}
END_TEST

START_TEST(test_simple_4_points_dfs) {
  write_test_file(TEST_FILE,
                  "4\n"
                  "0 0 0 1\n"
                  "0 0 0 1\n"
                  "0 0 0 1\n"
                  "1 1 1 0\n");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_depth_first_search(g, 2);

  ck_assert_uint_eq(result[0], 2);
  ck_assert_uint_eq(result[1], 4);
  ck_assert_uint_eq(result[2], 1);
  ck_assert_uint_eq(result[3], 3);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_complete_graphs_dfs) {
  write_test_file(TEST_FILE,
                  "6\n"
                  "0 1 1 1 1 1\n"
                  "1 0 1 1 1 1\n"
                  "1 1 0 1 1 1\n"
                  "1 1 1 0 1 1\n"
                  "1 1 1 1 0 1\n"
                  "1 1 1 1 1 0\n");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_depth_first_search(g, 5);

  ck_assert_uint_eq(result[0], 5);
  ck_assert_uint_eq(result[1], 1);
  ck_assert_uint_eq(result[2], 2);
  ck_assert_uint_eq(result[3], 3);
  ck_assert_uint_eq(result[4], 4);
  ck_assert_uint_eq(result[5], 6);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_orient_grapt_dfs) {
  write_test_file(TEST_FILE,
                  "5\n"
                  "0 0 1 0 0\n"
                  "1 0 0 0 0\n"
                  "0 1 0 0 1\n"
                  "0 1 0 0 0\n"
                  "0 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_depth_first_search(g, 3);

  ck_assert_uint_eq(result[0], 3);
  ck_assert_uint_eq(result[1], 2);
  ck_assert_uint_eq(result[2], 1);
  ck_assert_uint_eq(result[3], 5);
  ck_assert_uint_eq(result[4], 4);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_big_graph_dfs) {
  write_test_file(TEST_FILE,
                  "8\n"
                  "0 1 1 1 1 0 0 0\n"
                  "1 0 1 0 0 0 0 0\n"
                  "1 1 0 1 0 1 1 0\n"
                  "1 0 1 0 0 0 0 1\n"
                  "1 0 0 0 0 0 0 0\n"
                  "0 0 1 0 0 0 0 0\n"
                  "0 0 1 0 0 0 0 1\n"
                  "0 0 0 1 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_depth_first_search(g, 1);

  ck_assert_uint_eq(result[0], 1);
  ck_assert_uint_eq(result[1], 2);
  ck_assert_uint_eq(result[2], 3);
  ck_assert_uint_eq(result[3], 4);
  ck_assert_uint_eq(result[4], 8);
  ck_assert_uint_eq(result[5], 7);
  ck_assert_uint_eq(result[6], 6);
  ck_assert_uint_eq(result[7], 5);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_disconnected_dfs) {
  write_test_file(TEST_FILE,
                  "5\n"
                  "0 1 1 0 0\n"
                  "1 0 1 0 0\n"
                  "1 1 0 0 0\n"
                  "0 0 0 0 1\n"
                  "0 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_depth_first_search(g, 2);

  ck_assert_uint_eq(result[0], 2);
  ck_assert_uint_eq(result[1], 1);
  ck_assert_uint_eq(result[2], 3);
  ck_assert_uint_eq(result[3], 0);
  ck_assert_uint_eq(result[4], 0);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

//  ТЕСТЫ: BFS

START_TEST(test_null_graph_bfs) {
  ck_assert_ptr_null(my_breadth_first_search(NULL, 2));
}
END_TEST

START_TEST(test_null_graph_matrix_bfs) {
  my_graph* g = malloc(sizeof(my_graph));
  g->matrix = NULL;
  g->is_directed = 0;
  g->vertex_count = 1;

  ck_assert_ptr_null(my_breadth_first_search(NULL, 2));

  free(g);
}
END_TEST

START_TEST(test_vertex_more_bfs) {
  my_graph* g = malloc(sizeof(my_graph));
  g->matrix = NULL;
  g->is_directed = 0;
  g->vertex_count = 1;

  ck_assert_ptr_null(my_breadth_first_search(NULL, 2));

  free(g);
}
END_TEST

START_TEST(test_simple_4_points_bfs) {
  write_test_file(TEST_FILE,
                  "4\n"
                  "0 0 0 1\n"
                  "0 0 0 1\n"
                  "0 0 0 1\n"
                  "1 1 1 0\n");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_breadth_first_search(g, 2);

  ck_assert_uint_eq(result[0], 2);
  ck_assert_uint_eq(result[1], 4);
  ck_assert_uint_eq(result[2], 1);
  ck_assert_uint_eq(result[3], 3);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_complete_graphs_bfs) {
  write_test_file(TEST_FILE,
                  "6\n"
                  "0 1 1 1 1 1\n"
                  "1 0 1 1 1 1\n"
                  "1 1 0 1 1 1\n"
                  "1 1 1 0 1 1\n"
                  "1 1 1 1 0 1\n"
                  "1 1 1 1 1 0\n");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_breadth_first_search(g, 5);

  ck_assert_uint_eq(result[0], 5);
  ck_assert_uint_eq(result[1], 1);
  ck_assert_uint_eq(result[2], 2);
  ck_assert_uint_eq(result[3], 3);
  ck_assert_uint_eq(result[4], 4);
  ck_assert_uint_eq(result[5], 6);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_orient_grapt_bfs) {
  write_test_file(TEST_FILE,
                  "5\n"
                  "0 0 1 0 0\n"
                  "1 0 0 0 0\n"
                  "0 1 0 0 1\n"
                  "0 1 0 0 0\n"
                  "0 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_breadth_first_search(g, 3);

  ck_assert_uint_eq(result[0], 3);
  ck_assert_uint_eq(result[1], 2);
  ck_assert_uint_eq(result[2], 5);
  ck_assert_uint_eq(result[3], 1);
  ck_assert_uint_eq(result[4], 4);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_big_graph_bfs) {
  write_test_file(TEST_FILE,
                  "8\n"
                  "0 1 1 1 1 0 0 0\n"
                  "1 0 1 0 0 0 0 0\n"
                  "1 1 0 1 0 1 1 0\n"
                  "1 0 1 0 0 0 0 1\n"
                  "1 0 0 0 0 0 0 0\n"
                  "0 0 1 0 0 0 0 0\n"
                  "0 0 1 0 0 0 0 1\n"
                  "0 0 0 1 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_breadth_first_search(g, 1);

  ck_assert_uint_eq(result[0], 1);
  ck_assert_uint_eq(result[1], 2);
  ck_assert_uint_eq(result[2], 3);
  ck_assert_uint_eq(result[3], 4);
  ck_assert_uint_eq(result[4], 5);
  ck_assert_uint_eq(result[5], 6);
  ck_assert_uint_eq(result[6], 7);
  ck_assert_uint_eq(result[7], 8);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

START_TEST(test_disconnected_bfs) {
  write_test_file(TEST_FILE,
                  "5\n"
                  "0 1 1 0 0\n"
                  "1 0 1 0 0\n"
                  "1 1 0 0 0\n"
                  "0 0 0 0 1\n"
                  "0 0 0 1 0");
  my_graph* g = my_graph_load_from_file(TEST_FILE);

  unsigned* result = my_breadth_first_search(g, 2);

  ck_assert_uint_eq(result[0], 2);
  ck_assert_uint_eq(result[1], 1);
  ck_assert_uint_eq(result[2], 3);
  ck_assert_uint_eq(result[3], 0);
  ck_assert_uint_eq(result[4], 0);

  free(result);
  my_graph_destroy(g);
  remove(TEST_FILE);
}
END_TEST

Suite* my_depth_first_search_suite(void) {
  Suite* s = suite_create("depth_first_search");

  TCase* tc_errors_dfs = tcase_create("Errors");
  tcase_add_test(tc_errors_dfs, test_null_graph_dfs);
  tcase_add_test(tc_errors_dfs, test_null_graph_matrix_dfs);
  tcase_add_test(tc_errors_dfs, test_vertex_more_dfs);
  suite_add_tcase(s, tc_errors_dfs);

  TCase* tc_standart_dfs = tcase_create("Standart");
  tcase_add_test(tc_standart_dfs, test_simple_4_points_dfs);
  tcase_add_test(tc_standart_dfs, test_complete_graphs_dfs);
  tcase_add_test(tc_standart_dfs, test_big_graph_dfs);
  suite_add_tcase(s, tc_standart_dfs);

  TCase* tc_nostandart_dfs = tcase_create("nostandart");
  tcase_add_test(tc_nostandart_dfs, test_orient_grapt_dfs);
  tcase_add_test(tc_nostandart_dfs, test_disconnected_dfs);
  suite_add_tcase(s, tc_nostandart_dfs);

  return s;
}

Suite* my_breadth_first_search_suite(void) {
  Suite* s = suite_create("breadth_first_search");

  TCase* tc_errors_bfs = tcase_create("Errors");
  tcase_add_test(tc_errors_bfs, test_null_graph_bfs);
  tcase_add_test(tc_errors_bfs, test_null_graph_matrix_bfs);
  tcase_add_test(tc_errors_bfs, test_vertex_more_bfs);
  suite_add_tcase(s, tc_errors_bfs);

  TCase* tc_standart_bfs = tcase_create("Standart");
  tcase_add_test(tc_standart_bfs, test_simple_4_points_bfs);
  tcase_add_test(tc_standart_bfs, test_complete_graphs_bfs);
  tcase_add_test(tc_standart_bfs, test_big_graph_bfs);
  suite_add_tcase(s, tc_standart_bfs);

  TCase* tc_nostandart_bfs = tcase_create("nostandart");
  tcase_add_test(tc_nostandart_bfs, test_orient_grapt_bfs);
  tcase_add_test(tc_nostandart_bfs, test_disconnected_bfs);
  suite_add_tcase(s, tc_nostandart_bfs);

  return s;
}

Suite* solve_traveling_salesman_problem_suite(void) {
  Suite* s = suite_create("solve_traveling_salesman_problem");

  TCase* tc_tsp = tcase_create("TSP_Ant_Colony");

  tcase_add_test(tc_tsp, test_tsp_null_input);
  tcase_add_test(tc_tsp, test_tsp_empty_graph);
  tcase_add_test(tc_tsp, test_tsp_one_vertex);
  tcase_add_test(tc_tsp, test_tsp_two_vertices);
  tcase_add_test(tc_tsp, test_tsp_disconnected_graph);
  tcase_add_test(tc_tsp, test_tsp_known_optimal_tour);

  suite_add_tcase(s, tc_tsp);
  return s;
}

Suite* my_graph_shortest_path_suite(void) {
  Suite* s = suite_create("get_shortest_path_between_vertices");

  TCase* tc_invalid = tcase_create("Invalid_Input");
  tcase_add_test(tc_invalid, test_null_graph);
  tcase_add_test(tc_invalid, test_negative_vertex1);
  tcase_add_test(tc_invalid, test_negative_vertex2);
  tcase_add_test(tc_invalid, test_vertex1_out_of_bounds);
  tcase_add_test(tc_invalid, test_vertex2_out_of_bounds);
  suite_add_tcase(s, tc_invalid);

  TCase* tc_edge = tcase_create("Edge_Cases");
  tcase_add_test(tc_edge, test_same_vertex);
  tcase_add_test(tc_edge, test_single_vertex_graph);
  tcase_add_test(tc_edge, test_no_edge_between_vertices);
  suite_add_tcase(s, tc_edge);

  TCase* tc_undirected = tcase_create("Undirected_Graphs");
  tcase_add_test(tc_undirected, test_simple_undirected_path);
  tcase_add_test(tc_undirected, test_shortest_of_two_paths_undirected);
  suite_add_tcase(s, tc_undirected);

  TCase* tc_directed = tcase_create("Directed_Graphs");
  tcase_add_test(tc_directed, test_complex_directed_graph);
  tcase_add_test(tc_directed, test_directed_no_reverse_path);
  tcase_add_test(tc_directed, test_graph_with_cycle);
  suite_add_tcase(s, tc_directed);

  TCase* tc_special = tcase_create("Special_Scenarios");
  tcase_add_test(tc_special, test_disconnected_graph);
  tcase_add_test(tc_special, test_large_weights_no_overflow);
  tcase_add_test(tc_special, test_full_graph_shortcut);
  suite_add_tcase(s, tc_special);

  return s;
}

Suite* my_floyd_warshall_suite(void) {
  Suite* s = suite_create("Floyd_Warshall_Algorithm");

  TCase* tc_edge = tcase_create("Edge_Cases");
  tcase_add_test(tc_edge, test_fw_single_vertex);
  tcase_add_test(tc_edge, test_fw_two_vertices_no_edge);
  suite_add_tcase(s, tc_edge);

  TCase* tc_logic = tcase_create("Core_Logic");
  tcase_add_test(tc_logic, test_fw_simple_triangle_relaxation);
  suite_add_tcase(s, tc_logic);

  TCase* tc_complex = tcase_create("Complex_Scenarios");
  tcase_add_test(tc_complex, test_fw_disconnected_components);
  tcase_add_test(tc_complex, test_fw_multiple_intermediates);
  tcase_add_test(tc_complex, test_fw_protection_against_overflow);
  tcase_add_test(tc_complex, test_fw_cycle_does_not_break);
  suite_add_tcase(s, tc_complex);

  return s;
}

Suite* my_mst_suite(void) {
  Suite* s = suite_create("get_least_spanning_tree");

  TCase* tc_invalid = tcase_create("Invalid_Input");
  tcase_add_test(tc_invalid, test_mst_null_graph);
  tcase_add_test(tc_invalid, test_mst_empty_graph);
  suite_add_tcase(s, tc_invalid);

  TCase* tc_edge = tcase_create("Edge_Cases");
  tcase_add_test(tc_edge, test_mst_single_vertex);
  tcase_add_test(tc_edge, test_mst_two_vertices);
  tcase_add_test(tc_edge, test_mst_disconnected_graph);
  suite_add_tcase(s, tc_edge);

  TCase* tc_logic = tcase_create("Core_Logic");
  tcase_add_test(tc_logic, test_mst_simple_triangle);
  tcase_add_test(tc_logic, test_mst_linear_with_shortcut);
  tcase_add_test(tc_logic, test_mst_fully_connected);
  suite_add_tcase(s, tc_logic);

  return s;
}

int main(void) {
  int number_failed;

  SRunner* sr = srunner_create(my_graph_shortest_path_suite());
  srunner_add_suite(sr, my_depth_first_search_suite());
  srunner_add_suite(sr, my_breadth_first_search_suite());
  srunner_add_suite(sr, my_floyd_warshall_suite());
  srunner_add_suite(sr, my_graph_shortest_path_suite());
  srunner_add_suite(sr, my_floyd_warshall_suite());
  srunner_add_suite(sr, my_mst_suite());
  srunner_add_suite(sr, solve_traveling_salesman_problem_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}