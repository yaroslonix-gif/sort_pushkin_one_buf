#include "qsort_prog.h"


const struct ptr_sort_func sort_funcs[] = {{qsort, "standard_qsort"}, 
                                           {run_qsort, "custom_qsort"}};

const size_t sort_funcs_sz = sizeof(sort_funcs) / sizeof(sort_funcs[0]);

const struct ptr_comporator_func comporator_funcs[] = {my_compare_string_up, compare_string_up_from_right_to_left, compare_string_by_ptr};

const size_t comporator_funcs_sz = sizeof(comporator_funcs) / sizeof(comporator_funcs[0]);


int main(int argc, char* argv[]) {
    START_LOGG;

    clock_t start = clock();

    struct ptr_sort_func sort_func = sort_funcs[0];
    if (argc == 2)
        sort_func = get_sort_func(argv[1]);
    
    PRINT_COLOR(GREEN_TEXT, "Using %s mode\n", sort_func.func_name);
    
    sort_pushkin(sort_func);

    clock_t end = clock();
    
    printf("Time: %.3lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    STOP_LOGG;
    return 0;
}


/////////////////////////////////////////////////////////// ФУНКЦИИ ///////////////////////////////////////////////////////////
void sort_pushkin(struct ptr_sort_func sort_func) {
    ALOGG;
    
    size_t n_lines = 0;
    
    struct String* data_ind = read_file_by_lines_one_buf(PUSHKIN_TEXT, &n_lines);
    char* data_for_clear = data_ind[0].str;

    FILE* file_out = fopen(OUT_FILE_NAME, "w");
    ASSERT(file_out != NULL, NULL_CANT_OPEN_FILE);

    // По алфавиту слева направо, по алфавиту справа на лево, по указателям (по возрастанию)
    for (size_t i = 0; i < comporator_funcs_sz; i++) {
        sort_func.sort_func(data_ind, n_lines, sizeof(data_ind[0]), comporator_funcs[i].comporator_func);
        print_text_with_ptr(file_out, data_ind, n_lines);
        print_line(file_out);
    }

    fclose(file_out);

    // Free
    free(data_for_clear);
    free(data_ind);

    fprintf(stderr, "end\n"); 
}


/////////////////////////////////////////////////////////// АЛГОРИТМ QSORT ///////////////////////////////////////////////////////////
void run_qsort(void* data, size_t data_sz, size_t elem_sz, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ALOGG;
    ASSERT(data != NULL, NULL_ERROR);
    ASSERT(comparator != NULL, NULL_ERROR);

    my_qsort(data, data_sz, elem_sz, 0, data_sz - 1, comparator);
}

void my_qsort(void* data, const size_t data_sz, const size_t elem_sz, int left, int right, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ALOGG;
    ASSERT(data != NULL, NULL_ERROR);
    ASSERT(comparator != NULL, NULL_ERROR);

    if (right <= left) return;

    size_t last_left = left;
    size_t last_right = right;
    
    //debug_qsort_print(data, left, right, data_sz, "Before swap_qsort");
    swap_qsort(data, data_sz, elem_sz, &left, &right, comparator);
    //debug_qsort_print(data, left, right, data_sz, "After swap_qsort");

    my_qsort(data, data_sz, elem_sz, last_left, right, comparator);
    my_qsort(data, data_sz, elem_sz, right + 1, last_right, comparator);
}

void swap_qsort(void* data, const size_t data_sz, const size_t elem_sz, int* left, int* right, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ASSERT(data  != NULL, NULL_ERROR);
    ASSERT(left  != NULL, NULL_ERROR);
    ASSERT(right != NULL, NULL_ERROR);
    ASSERT(comparator != NULL, NULL_ERROR);

    size_t mid_ind = (*left + *right) / 2;

    while (*left <= *right) {
        while (comparator((char*)data + *left * elem_sz, (char*)data + mid_ind * elem_sz) < 0) {
            (*left)++;
        }
        while (comparator((char*)data + *right * elem_sz, (char*)data + mid_ind * elem_sz) > 0) {
            (*right)--;
        }

        if (*left < *right) {

            universal_swap((char*)data + *left * elem_sz, (char*)data + *right * elem_sz, elem_sz);

            if (*left == mid_ind)
                mid_ind = *right;
            else if (*right == mid_ind)
                mid_ind = *left;
            
            (*left)++;
            (*right)--;
            //debug_qsort_print(data, *left, *right, data_sz, "After swap elements");
        } else {
            break;
        }
    }    
}

/////////////////////////////////////////////////////////// КОМПОРАТОРЫ ///////////////////////////////////////////////////////////
int my_compare_string_up(const void* ptr_a, const void* ptr_b) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);

    if (ptr_a == ptr_b) return EQUAL_COMPARE; // если переданы одинаковые указатели на строки, то строки равны 

    struct String str_a = *(struct String*)ptr_a;
    struct String str_b = *(struct String*)ptr_b;

    int compared = 0; // если сравнений не было, то строка является незначащей и отправляется в конец сортируемых данных

    size_t ind_a = 0, ind_b = 0;
    for ( ;ind_a < str_a.len && ind_b < str_b.len; ind_a++, ind_b++) {
        while (!isalpha(str_a.str[ind_a]) && (ind_a < str_a.len)) ind_a++;
        while (!isalpha(str_b.str[ind_b]) && (ind_b < str_b.len)) ind_b++;

        if (ind_a == str_a.len || ind_b == str_b.len)
            break;
        
        compared = 1;
        
        if (tolower(str_a.str[ind_a]) == tolower(str_b.str[ind_b]))
            continue;
        
        if (tolower(str_a.str[ind_a]) < tolower(str_b.str[ind_b]))
            return LESS_COMPARE;

        return MORE_COMPARE;
    }

    // check last char
    if (tolower(str_a.str[ind_a]) < tolower(str_b.str[ind_b])) {
        if (compared == 1)
            return LESS_COMPARE;
        
        return MORE_COMPARE;
    }
    
    if (tolower(str_b.str[ind_b]) < tolower(str_a.str[ind_a])) {
        if (compared == 1)
            return MORE_COMPARE;
        
        return LESS_COMPARE;
    }
    
    return EQUAL_COMPARE;
}

int compare_string_up_from_right_to_left(const void* ptr_a, const void* ptr_b) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);

    struct String str_a = *(struct String*)ptr_a;
    struct String str_b = *(struct String*)ptr_b;
    
    int ind_a = str_a.len - 1;
    int ind_b = str_b.len - 1;
    int compared = 0; // если сравнений не было, то строка является незначащей и отправляется в конец сортируемых данных

    for ( ;ind_a >= 0 && ind_b >= 0; ind_a--, ind_b--) {
        while (ind_a >= 0 && !isalpha(str_a.str[ind_a])) ind_a--;
        while (ind_b >= 0 && !isalpha(str_b.str[ind_b])) ind_b--;

        if (ind_a < 0 || ind_b < 0)
            break;

        compared = 1;

        if (tolower(str_a.str[ind_a]) == tolower(str_b.str[ind_b]))
            continue;
        
        if (tolower(str_a.str[ind_a]) < tolower(str_b.str[ind_b]))
            return LESS_COMPARE;
        
        return MORE_COMPARE;
    }

    if (ind_a < 0 && ind_b >= 0) {
        if (compared == 1)
            return LESS_COMPARE;
        
        return MORE_COMPARE;
    }

    if (ind_a >= 0 && ind_b < 0) {
        if (compared == 1)
            return MORE_COMPARE;
        
        return LESS_COMPARE;
    }

    return EQUAL_COMPARE;
}

int compare_string_by_ptr(const void* ptr_a, const void* ptr_b) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);
    
    char* a_str = ((struct String*)ptr_a)->str;
    char* b_str = ((struct String*)ptr_b)->str;

    if (a_str < b_str)
        return LESS_COMPARE;
    
    if (a_str > b_str)
        return MORE_COMPARE;
    
    return EQUAL_COMPARE;
}

/////////////////////////////////////////////////////////// ЧТЕНИЕ СТРОК ///////////////////////////////////////////////////////////
char* read_line(FILE* source) {
    ASSERT(source != NULL, NULL_ERROR);

    size_t buf_sz = BASE_INIT_ARRAY_LENGTH;
    size_t now_num_ch = 0;
    char *buf = (char*)calloc(buf_sz, sizeof(char));
    ASSERT(buf != NULL, NULL_MEMORY_ERR);

    for (char ch = '\0'; (ch = fgetc(source)) != '\n'; now_num_ch++) {
        if (feof(source)) {
            if (now_num_ch != 0)
                return buf;
            
            return NULL;
        }
        if (now_num_ch == buf_sz) {
            buf_sz *= 2;
            buf = (char*)realloc(buf, buf_sz * sizeof(char));
            ASSERT(buf != NULL, NULL_MEMORY_ERR);
        }
        buf[now_num_ch] = ch;
    }

    if (now_num_ch == buf_sz) {
        buf_sz *= 2;
        buf = (char*)realloc(buf, buf_sz);
        ASSERT(buf != NULL, NULL_MEMORY_ERR);
    }

    buf[now_num_ch] = '\0';
    now_num_ch++;
    buf = (char*)realloc(buf, now_num_ch);
    ASSERT(buf != NULL, NULL_MEMORY_ERR);

    return buf;
}

struct String* read_file_by_lines(const char* file_name, size_t* num_of_lines) {
    ASSERT(file_name != NULL, NULL_ERROR);
    ASSERT(num_of_lines != NULL, NULL_ERROR);

    FILE* file_in = fopen(file_name, "r");
    ASSERT(file_in != NULL, NULL_CANT_OPEN_FILE);

    size_t n_of_lines = 0;
    size_t sz_data_ind = BASE_INIT_ARRAY_LENGTH;

    struct String* data_ind = (struct String*)calloc(sz_data_ind, sizeof(struct String));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    while(!feof(file_in)) {
        if (n_of_lines == sz_data_ind) {
            sz_data_ind *= 2;
            data_ind = (struct String*)realloc(data_ind, sz_data_ind * sizeof(struct String));
            ASSERT(data_ind != NULL, NULL_MEMORY_ERR);
        }

        data_ind[n_of_lines].str = read_line(file_in);
        data_ind[n_of_lines].len = str_len(data_ind[n_of_lines].str);

        if (data_ind[n_of_lines].str != NULL) {
            n_of_lines++;
        }
    }

    data_ind = (struct String*)realloc(data_ind, n_of_lines * sizeof(struct String));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    *num_of_lines = n_of_lines;

    fclose(file_in);
    return data_ind;
}

char* read_file_in_one_buf(const int file_in, size_t* sz_text) {
    ASSERT(file_in != -1, NULL_CANT_OPEN_FILE);
    ASSERT(sz_text != NULL, NULL_ERROR);

    size_t sz_data = get_file_size(file_in);
    char* data = (char*)calloc(sz_data + 1, sizeof(char));
    ASSERT(data != NULL, NULL_MEMORY_ERR);

    // realloc for \r
    *sz_text = read(file_in, data, sz_data);
    data = (char*)realloc(data, (*sz_text + 1) * sizeof(char));
    ASSERT(data != NULL, NULL_MEMORY_ERR);

    data[*sz_text] = '\0'; // add symbol end of string

    return data;
}

struct String* pars_text_to_lines(char* data, const size_t sz_text, size_t* num_of_lines) {
    ASSERT(data != NULL, NULL_ERROR);
    ASSERT(num_of_lines != NULL, NULL_ERROR);

    size_t sz_data_ind = count_symbolsin_text(data, '\n') + 1;
    struct String* data_ind = (struct String*)calloc(sz_data_ind, sizeof(struct String));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    size_t n_of_lines = 0;

    //==================================== Парсинг текста ====================================//
    char* last_ind = data;
    for (size_t i = 0; i <= sz_text; i++) {
        // if (n_of_lines == sz_data_ind) {
        //     sz_data_ind *= 2;
        //     data_ind = (struct String*)realloc(data_ind, sz_data_ind * sizeof(struct String));
        //     ASSERT(data_ind != NULL, NULL_MEMORY_ERR);
        // }

        if (data[i] == '\n' || data[i] == '\0') {
            data_ind[n_of_lines].str = last_ind;
            data_ind[n_of_lines].len = (size_t)(&data[i] - last_ind);
            last_ind = data + i + 1;
            n_of_lines += 1;
            data[i] = '\0';
        }
    }

    // уменьшение массива для оптимизации занимаемой памяти
    // data_ind = (struct String*)realloc(data_ind, n_of_lines * sizeof(struct String));
    // ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    *num_of_lines = n_of_lines;

    return data_ind;
}

struct String* read_file_by_lines_one_buf(const char* file_name, size_t* num_of_lines) {
    ASSERT(file_name != NULL, NULL_ERROR);
    ASSERT(num_of_lines != NULL, NULL_ERROR);

    int file_in = open(file_name, O_RDONLY);
    ASSERT(file_in != -1, NULL_CANT_OPEN_FILE);

    size_t sz_text = 0;
    char* data = read_file_in_one_buf(file_in, &sz_text);

    struct String* data_ind = pars_text_to_lines(data, sz_text, num_of_lines);

    close(file_in);
    return data_ind;
}

/////////////////////////////////////////////////////////// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ///////////////////////////////////////////////////////////
size_t str_len(const char *str) {
    ASSERT(str != NULL, NULL_ERROR);

    size_t len = 0;
    while (*str++ != '\0')
        len++;
    
    return len;
}

void print_text_with_ptr(FILE* file_out, const struct String* data, const size_t n_lines) {
    ASSERT(file_out != NULL, NULL_ERROR);
    ASSERT(data != NULL, NULL_ERROR);

    for (size_t i = 0; i < n_lines; i++) {
        fprintf(file_out, "< %6d >:< %p >:<%s>\n", i, &data[i].str, data[i].str);
    }
}

void debug_qsort_print(struct String* data, size_t left, size_t right, size_t data_sz, char* reason_of_func_call) {
    ASSERT(data != NULL, NULL_ERROR);
    ASSERT(reason_of_func_call != NULL, NULL_ERROR);

    print_line_color(stdout, YELLOW_TEXT);
    PRINT_COLOR(RESET_COLOR_TEXT, "%s\n", reason_of_func_call);
    
    for (size_t i = 0; i < data_sz; i++)
        PRINT_COLOR(GREEN_TEXT, "%10d ", i);
    
    printf("\n");

    for (size_t i = 0; i < left; i++)
        PRINT_COLOR(TURQUOISE_TEXT, "%10s ", data[i].str);
    
    for (size_t i = left; i < right; i++)
        PRINT_COLOR(RESET_COLOR_TEXT, "%10s ", data[i].str);

    
    for (int i = (left > right) ? right + 1 : right; i < data_sz; i++)
        PRINT_COLOR(RED_TEXT, "%10s ", data[i].str);
    
    PRINT_COLOR(GREEN_TEXT, "    left = " TURQUOISE_TEXT "%d" GREEN_TEXT "; right = " RED_TEXT "%d\n", left, right);

    if (left == right) {
        for (size_t i = 0; i < data_sz; i++) {
            if (i == left) {
                PRINT_COLOR(TURQUOISE_TEXT, "%9c", 'L');
                PRINT_COLOR(RED_TEXT, "R ");
            } else {
                printf("%10c ", ' ');
            }
        }
    } else {
        for (size_t i = 0; i < data_sz; i++) { // do krasivo
            if (i == left) {
                PRINT_COLOR(TURQUOISE_TEXT, "%10s ", "L");
            } else if (i == right) {
                PRINT_COLOR(RED_TEXT, "%10s ", "R");
            } else {
                printf("%10c ", ' ');
            }
        }
    }
    
    printf("\n");
}

struct ptr_sort_func get_sort_func(const char* func_name) {
    ASSERT(func_name != NULL, NULL_ERROR);

    for (size_t i = 0; i < sort_funcs_sz; i++) {
        if (strcmp(func_name, sort_funcs[i].func_name) == 0)
            return sort_funcs[i];
    }

    return sort_funcs[0];
}

size_t count_symbolsin_text(const char* text, const char targ_ch) {
    ASSERT(text != NULL, NULL_ERROR);

    size_t cnt_targ_ch = 0;

    for (;*text != '\0'; text++)
        if (*text == targ_ch)
            cnt_targ_ch++;
    
    return cnt_targ_ch;
}