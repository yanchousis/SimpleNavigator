#ifndef SRC_MY_GRAPH_H_
#define SRC_MY_GRAPH_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

/** @brief Структура графа, хранящая информацию через матрицу смежности. */
typedef struct {
  int** matrix;      ///< Матрица смежности (0 — ребра нет, иначе вес).
  int vertex_count;  ///< Количество вершин в графе.
  bool is_directed;  ///< true — ориентированный, false — неориентированный.
} my_graph;

/**
 * @brief Создаёт пустой граф с заданным количеством вершин.
 * @param vertex_count Количество вершин (должно быть > 0).
 * @return Указатель на созданный граф или NULL при ошибке.
 */
my_graph* my_graph_create(int vertex_count);

/**
 * @brief Освобождает ресурсы, занятые графом.
 * @param graph Указатель на граф.
 */
void my_graph_destroy(my_graph* graph);

/**
 * @brief Загружает граф из файла в формате матрицы смежности.
 *
 * Формат файла: первая строка — количество вершин, далее N строк по N чисел.
 * Ориентированность определяется автоматически по симметрии матрицы.
 *
 * @param filename Путь к файлу.
 * @return Указатель на загруженный граф или NULL при ошибке.
 */
my_graph* my_graph_load_from_file(const char* filename);

/**
 * @brief Экспортирует граф в файл в формате DOT (Graphviz).
 * @param graph Указатель на граф.
 * @param filename Путь к выходному файлу.
 * @return 0 при успехе, -1 при ошибке.
 */
int my_graph_export_to_dot(const my_graph* graph, const char* filename);

/**
 * @brief Добавляет ребро между вершинами.
 * @return 0 при успехе, -1 при некорректных индексах.
 */
int my_graph_add_edge(my_graph* graph, int from, int to, int weight);

/**
 * @brief Удаляет ребро между вершинами.
 * @return 0 при успехе, -1 при некорректных индексах.
 */
int my_graph_remove_edge(my_graph* graph, int from, int to);

/**
 * @brief Проверяет наличие ребра.
 * @return true, если ребро существует.
 */
bool my_graph_has_edge(const my_graph* graph, int from, int to);

/**
 * @brief Возвращает вес ребра (0, если ребра нет).
 */
int my_graph_get_weight(const my_graph* graph, int from, int to);

/**
 * @brief Возвращает количество вершин.
 */
int my_graph_get_vertex_count(const my_graph* graph);

/**
 * @brief Проверяет, является ли граф ориентированным.
 */
bool my_graph_is_directed(const my_graph* graph);

int** allocate_matrix(int n);

double** allocate_matrix_double(int n);

void free_matrix(int** matrix, int n);

void free_matrix_double(double** matrix, int n);

#endif  // SRC_MY_GRAPH_H_