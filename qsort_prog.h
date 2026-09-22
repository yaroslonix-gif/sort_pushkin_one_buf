#include "base.h"


/////////////////////////////////////////////////////////// DEFINE ///////////////////////////////////////////////////////////
#define PUSHKIN_TEXT  "Onegin.txt"
#define OUT_FILE_NAME "out.txt"


/////////////////////////////////////////////////////////// STRUCT ///////////////////////////////////////////////////////////
struct ptr_sort_func {
    void (*sort_func)(void* data, const size_t data_sz, const size_t elem_sz, int (*comparator)(const void* ptr_a, const void* ptr_b));
    char* func_name;
};


/////////////////////////////////////////////////////////// КОНСТАНТЫ ///////////////////////////////////////////////////////////
extern const struct ptr_sort_func sort_funcs[];
extern const size_t sort_funcs_sz;


/////////////////////////////////////////////////////////// ФУНКЦИИ ///////////////////////////////////////////////////////////
void sort_pushkin(struct ptr_sort_func sort_func);


/////////////////////////////////////////////////////////// АЛГОРИТМ QSORT ///////////////////////////////////////////////////////////
void run_qsort(void* data, const size_t data_sz, const size_t elem_sz, int (*comparator)(const void* ptr_a, const void* ptr_b));

void my_qsort(void* data, const size_t data_sz, const size_t elem_sz, int left, int right, int (*comparator)(const void* ptr_a, const void* ptr_b));

void swap_qsort(void* data, const size_t data_sz, const size_t elem_sz, int* left, int* right, int (*comparator)(const void* ptr_a, const void* ptr_b));


/////////////////////////////////////////////////////////// КОМПОРАТОРЫ ///////////////////////////////////////////////////////////
int my_compare_string_up(const void* ptr_a, const void* ptr_b);

int compare_string_up_from_right_to_left(const void* ptr_a, const void* ptr_b);

int compare_string_by_ptr(const void* ptr_a, const void* ptr_b);


/////////////////////////////////////////////////////////// ЧТЕНИЕ СТРОК ///////////////////////////////////////////////////////////
char* read_line(FILE* source);

struct String* read_file_by_lines(const char* file_name, size_t* num_of_lines);

struct String* read_file_by_lines_one_buf(const char* file_name, size_t* num_of_lines);


/////////////////////////////////////////////////////////// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ///////////////////////////////////////////////////////////
size_t str_len(const char *str);

void print_text_with_ptr(FILE* file_out, const struct String* data, const size_t n_lines);

void debug_qsort_print(struct String* data, size_t left, size_t right, size_t data_sz, char* reason_of_func_call);

struct ptr_sort_func get_sort_func(const char* func_name);