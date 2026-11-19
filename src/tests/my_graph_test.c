#include "../my_graph.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNDIRECTED_FILE "graph_undirected.txt"
#define DIRECTED_FILE "graph_directed.txt"
#define MISSING_FILE "does_not_exist.txt"
#define DOT_OUT_FILE "out.dot"

static void write_test_file(const char* path, const char* content) {
  FILE* fp = fopen(path, "w");
  ck_assert_ptr_nonnull(fp);
  fputs(content, fp);
  fclose(fp);
}

/* ---------- create / destroy ---------- */

START_TEST(test_create_valid) {
  my_graph* g = my_graph_create(5);
  ck_assert_ptr_nonnull(g);
  ck_assert_int_eq(my_graph_get_vertex_count(g), 5);
  ck_assert(!my_graph_is_directed(g));
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_create_invalid) {
  ck_assert_ptr_null(my_graph_create(0));
  ck_assert_ptr_null(my_graph_create(-3));
}
END_TEST

START_TEST(test_destroy_null) { my_graph_destroy(NULL); }
END_TEST

/* ---------- load from file ---------- */

START_TEST(test_load_undirected) {
  write_test_file(UNDIRECTED_FILE,
                  "4\n"
                  "0 1 1 0\n"
                  "1 0 0 1\n"
                  "1 0 0 1\n"
                  "0 1 1 0\n");
  my_graph* g = my_graph_load_from_file(UNDIRECTED_FILE);
  ck_assert_ptr_nonnull(g);
  ck_assert_int_eq(my_graph_get_vertex_count(g), 4);
  ck_assert(!my_graph_is_directed(g));
  ck_assert(my_graph_has_edge(g, 0, 1));
  ck_assert(my_graph_has_edge(g, 1, 0));
  ck_assert(!my_graph_has_edge(g, 0, 3));
  my_graph_destroy(g);
  remove(UNDIRECTED_FILE);
}
END_TEST

START_TEST(test_load_directed) {
  write_test_file(DIRECTED_FILE,
                  "3\n"
                  "0 1 0\n"
                  "0 0 1\n"
                  "1 0 0\n");
  my_graph* g = my_graph_load_from_file(DIRECTED_FILE);
  ck_assert_ptr_nonnull(g);
  ck_assert_int_eq(my_graph_get_vertex_count(g), 3);
  ck_assert(my_graph_is_directed(g));
  ck_assert(my_graph_has_edge(g, 0, 1));
  ck_assert(!my_graph_has_edge(g, 1, 0));
  my_graph_destroy(g);
  remove(DIRECTED_FILE);
}
END_TEST

START_TEST(test_load_invalid) {
  ck_assert_ptr_null(my_graph_load_from_file(NULL));
  ck_assert_ptr_null(my_graph_load_from_file(MISSING_FILE));
}
END_TEST

START_TEST(test_load_malformed) {
  const char* path = "bad.txt";
  write_test_file(path, "3\n0 1\n1 0 0\n0 0 0\n");
  ck_assert_ptr_null(my_graph_load_from_file(path));
  remove(path);
}
END_TEST

/* ---------- edges ---------- */

START_TEST(test_add_remove_edge) {
  my_graph* g = my_graph_create(3);
  ck_assert_int_eq(my_graph_add_edge(g, 0, 1, 5), 0);
  ck_assert(my_graph_has_edge(g, 0, 1));
  ck_assert_int_eq(my_graph_get_weight(g, 0, 1), 5);
  // Неориентированный — симметрия.
  ck_assert(my_graph_has_edge(g, 1, 0));
  ck_assert_int_eq(my_graph_add_edge(g, 0, 1, 0), -1);  // weight=0
  ck_assert_int_eq(my_graph_add_edge(g, 5, 1, 1), -1);  // out of range
  ck_assert_int_eq(my_graph_remove_edge(g, 0, 1), 0);
  ck_assert(!my_graph_has_edge(g, 0, 1));
  ck_assert(!my_graph_has_edge(g, 1, 0));
  my_graph_destroy(g);
}
END_TEST

START_TEST(test_remove_invalid) {
  my_graph* g = my_graph_create(2);
  ck_assert_int_eq(my_graph_remove_edge(g, 10, 0), -1);
  ck_assert_int_eq(my_graph_remove_edge(NULL, 0, 0), -1);
  my_graph_destroy(g);
}
END_TEST

/* ---------- export to dot ---------- */

START_TEST(test_export_undirected) {
  write_test_file(UNDIRECTED_FILE,
                  "3\n"
                  "0 1 0\n"
                  "1 0 1\n"
                  "0 1 0\n");
  my_graph* g = my_graph_load_from_file(UNDIRECTED_FILE);
  ck_assert_int_eq(my_graph_export_to_dot(g, DOT_OUT_FILE), 0);

  FILE* fp = fopen(DOT_OUT_FILE, "r");
  ck_assert_ptr_nonnull(fp);
  char buf[256] = {0};
  fread(buf, 1, sizeof(buf) - 1, fp);
  fclose(fp);
  ck_assert(strstr(buf, "graph {") != NULL);
  ck_assert(strstr(buf, "--") != NULL);
  my_graph_destroy(g);
  remove(UNDIRECTED_FILE);
  remove(DOT_OUT_FILE);
}
END_TEST

START_TEST(test_export_directed) {
  write_test_file(DIRECTED_FILE,
                  "2\n"
                  "0 1\n"
                  "0 0\n");
  my_graph* g = my_graph_load_from_file(DIRECTED_FILE);
  ck_assert_int_eq(my_graph_export_to_dot(g, DOT_OUT_FILE), 0);

  FILE* fp = fopen(DOT_OUT_FILE, "r");
  ck_assert_ptr_nonnull(fp);
  char buf[256] = {0};
  fread(buf, 1, sizeof(buf) - 1, fp);
  fclose(fp);
  ck_assert(strstr(buf, "digraph") != NULL);
  ck_assert(strstr(buf, "->") != NULL);
  my_graph_destroy(g);
  remove(DIRECTED_FILE);
  remove(DOT_OUT_FILE);
}
END_TEST

START_TEST(test_export_invalid) {
  ck_assert_int_eq(my_graph_export_to_dot(NULL, DOT_OUT_FILE), -1);
  my_graph* g = my_graph_create(2);
  ck_assert_int_eq(my_graph_export_to_dot(g, NULL), -1);
  my_graph_destroy(g);
}
END_TEST

Suite* my_graph_suite(void) {
  Suite* s = suite_create("my_graph");
  TCase* tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_valid);
  tcase_add_test(tc_core, test_create_invalid);
  tcase_add_test(tc_core, test_destroy_null);
  tcase_add_test(tc_core, test_load_undirected);
  tcase_add_test(tc_core, test_load_directed);
  tcase_add_test(tc_core, test_load_invalid);
  tcase_add_test(tc_core, test_load_malformed);
  tcase_add_test(tc_core, test_add_remove_edge);
  tcase_add_test(tc_core, test_remove_invalid);
  tcase_add_test(tc_core, test_export_invalid);
  tcase_add_test(tc_core, test_export_undirected);
  tcase_add_test(tc_core, test_export_directed);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  Suite* s = my_graph_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return failed == 0 ? 0 : 1;
}