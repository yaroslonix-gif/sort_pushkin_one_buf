#include "base.h"


void swap_qsort(void* data, const size_t sz, const size_t elem_sz, int* left, int* right, int (*comparator)(const void* ptr_a, const void* ptr_b));

void my_qsort(void* data, const size_t sz, const size_t elem_sz, int left, int right, int (*comparator)(const void* ptr_a, const void* ptr_b));

void run_qsort(void* data, const size_t sz, const size_t elem_sz, int (*comparator)(const void* ptr_a, const void* ptr_b));

void print_line(void);

int my_compare_string_up(const void* ptr_a, const void* ptr_b);

int compare_string_up_from_right_to_left(const void* ptr_a, const void* ptr_b);

int compare_by_ptr(const void* ptr_a, const void* ptr_b);

void universal_swap(const void* restrict ptr_a, const void* restrict ptr_b, size_t elem_sz);

char* read_line(FILE* source);

char** read_file_by_lines(const char* file_name, size_t* num_of_lines);

char** read_file_by_lines_one_buf(const char* file_name, size_t* num_of_lines);

void debug_qsort_print(char* data[], size_t left, size_t right, size_t sz, char* reason_of_func_call);

void sort_pushkin() {
    size_t n_lines = 0;
    
    char** data_ind = read_file_by_lines_one_buf("Onegin.txt", &n_lines);
    char* data_for_clear = data_ind[0];

    FILE* file_out = fopen("out.txt", "w");

    // По алфавиту
    run_qsort(data_ind, n_lines, sizeof(data_ind[0]), my_compare_string_up);
    
    for (size_t i = 0; i < n_lines; i++) {
        fprintf(file_out, "<%p>:<%s>\n", &data_ind[i], data_ind[i]);
    }

    // По алфовиту с конца строк
    run_qsort(data_ind, n_lines, sizeof(data_ind[0]), compare_string_up_from_right_to_left);
    
    for (size_t i = 0; i < n_lines; i++) {
        fprintf(file_out, "<%p>:<%s>\n", &data_ind[i], data_ind[i]);
    }

    // Исходный текст
    run_qsort(data_ind, n_lines, sizeof(data_ind[0]), compare_by_ptr);
    
    for (size_t i = 0; i < n_lines; i++) {
        fprintf(file_out, "<%p>:<%s>\n", &data_ind[i], data_ind[i]);
    }

    fclose(file_out);

    free(data_for_clear);
    free(data_ind);
    printf("end\n");
}


int main() {
    sort_pushkin();
}


void swap_qsort(void* data, const size_t sz, const size_t elem_sz, int* left, int* right, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ASSERT(data  != NULL, NULL_ERROR);
    ASSERT(left  != NULL, NULL_ERROR);
    ASSERT(right != NULL, NULL_ERROR);

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
            //PRINT_QSORT_DEBUG("After swap elements");
        } else {
            break;
        }
    }
    
}

void my_qsort(void* data, const size_t sz, const size_t elem_sz, int left, int right, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ASSERT(data != NULL, NULL_ERROR);

    if (right <= left) return;

    size_t last_left = left;
    size_t last_right = right;
    
    //PRINT_QSORT_DEBUG("Before swap_sort");
    swap_qsort(data, sz, elem_sz, &left, &right, comparator);
    //PRINT_QSORT_DEBUG("After swap_sort");

    my_qsort(data, sz, elem_sz, last_left, right, comparator);
    my_qsort(data, sz, elem_sz, right + 1, last_right, comparator);
}

void run_qsort(void* data, size_t sz, size_t elem_sz, int (*comparator)(const void* ptr_a, const void* ptr_b)) {
    ASSERT(data != NULL, NULL_ERROR);

    my_qsort(data, sz, elem_sz, 0, sz - 1, comparator);
}

int my_compare_string_up(const void* ptr_a, const void* ptr_b) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);

    if (ptr_a == ptr_b) return 0;
    char* str_a = *(char**)ptr_a;
    char* str_b = *(char**)ptr_b;
    int compared = 0;
    //printf("%s %s\n", str_a, str_b);

    for ( ;*str_a != '\0' && *str_b != '\0'; str_a++, str_b++) {
        while (!isalpha(*str_a) && (*str_a != '\0')) str_a++;
        while (!isalpha(*str_b) && (*str_b != '\0')) str_b++;

        if (*str_a == '\0' || *str_b == '\0')
            break;
        
        compared = 1;
        
        if (tolower(*str_a) == tolower(*str_b))
            continue;
        
        if (tolower(*str_a) < tolower(*str_b))
            return -1;

        return 1;
    }

    if (tolower(*str_a) < tolower(*str_b)) {
        if (compared == 1)
            return -1;
        
        return 1;
    }
    
    if (tolower(*str_b) < tolower(*str_a)) {
        if (compared == 1)
            return 1;
        
        return -1;
    }
    
    return 0;
}

int compare_string_up_from_right_to_left(const void* ptr_a, const void* ptr_b) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);

    char* str_a = *(char**)ptr_a;
    char* str_b = *(char**)ptr_b;
    int ind_a = strlen(str_a) - 1;
    int ind_b = strlen(str_b) - 1;
    int compared = 0;

    for ( ;ind_a >= 0 && ind_b >= 0; ind_a--, ind_b--) {
        while (ind_a >= 0 && !isalpha(str_a[ind_a]) && (str_a[ind_a] != '\0')) ind_a--;
        while (ind_b >= 0 && !isalpha(str_b[ind_b]) && (str_b[ind_b] != '\0')) ind_b--;

        if (str_a[ind_a] < 0 || str_b[ind_b] < 0)
            break;

        compared = 1;

        if (tolower(str_a[ind_a]) == tolower(str_b[ind_b]))
            continue;
        
        if (tolower(str_a[ind_a]) < tolower(str_b[ind_b]))
            return -1;
        
        return 1;
    }

    if (ind_a < ind_b)
        return -1;
    
    if (ind_a > ind_b)
        return 1;
    
    return 0;
}

void print_line(void) {
    printf(YELLOW);
    for (int i = 0; i < lEN_OF_PRINT_LINE; i++)
        printf("=");
    
    printf("%s\n", RESET_COLOR);
}

void debug_qsort_print(char* data[], size_t left, size_t right, size_t sz, char* reason_of_func_call) {
    ASSERT(data != NULL, NULL_ERROR);
    ASSERT(reason_of_func_call != NULL, NULL_ERROR);

    print_line();
    PRINT_COLOR(RESET_COLOR, "%s\n", reason_of_func_call);
    
    for (size_t i = 0; i < sz; i++)
        PRINT_COLOR(GREEN, "%10d ", i);
    
    printf("\n");

    for (size_t i = 0; i < left; i++)
        PRINT_COLOR(TURQUOISE, "%10s ", data[i]);
    
    for (size_t i = left; i < right; i++)
        PRINT_COLOR(RESET_COLOR, "%10s ", data[i]);

    
    for (int i = (left > right) ? right + 1 : right; i < sz; i++)
        PRINT_COLOR(RED, "%10s ", data[i]);
    
    PRINT_COLOR(GREEN, "    left = " TURQUOISE "%d" GREEN "; right = " RED "%d\n", left, right);

    if (left == right) {
        for (size_t i = 0; i < sz; i++) {
            if (i == left) {
                PRINT_COLOR(TURQUOISE, "%9c", 'L');
                PRINT_COLOR(RED, "R ");
            } else {
                printf("%10c ", ' ');
            }
        }
    } else {
        for (size_t i = 0; i < sz; i++) {
            if (i == left) {
                PRINT_COLOR(TURQUOISE, "%10s ", "L");
            } else if (i == right) {
                PRINT_COLOR(RED, "%10s ", "R");
            } else {
                printf("%10c ", ' ');
            }
        }
    }
    
    printf("\n");
}

void universal_swap(const void* restrict ptr_a, const void* restrict ptr_b, size_t elem_sz) {
    ASSERT(ptr_a != NULL, NULL_ERROR);
    ASSERT(ptr_b != NULL, NULL_ERROR);

    uint64_t buf[8] = {0};
    size_t buf_sz = sizeof(buf);
    char* a = (char*)ptr_a;
    char* b = (char*)ptr_b;

    while (elem_sz > 0) {
        buf_sz = MIN(buf_sz, elem_sz);

        memcpy(buf, b, buf_sz);
        memcpy(b, a, buf_sz);
        memcpy(a, buf, buf_sz);

        a += buf_sz;
        b += buf_sz;
        elem_sz -= buf_sz;
    }
}

char* read_line(FILE* source) {
    ASSERT(source != NULL, NULL_ERROR);

    size_t buf_sz = 32;
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

char** read_file_by_lines(const char* file_name, size_t* num_of_lines) {
    ASSERT(file_name != NULL, NULL_ERROR);
    ASSERT(num_of_lines != NULL, NULL_ERROR);

    FILE* file_in = fopen(file_name, "r");
    ASSERT(file_in != NULL, NULL_CANT_OPEN_FILE);

    *num_of_lines = 0;
    size_t sz_data_ind = 32;
    char** data_ind = (char**)calloc(sz_data_ind, sizeof(char*));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    while(!feof(file_in)) {
        if (*num_of_lines == sz_data_ind) {
            sz_data_ind *= 2;
            data_ind = (char**)realloc(data_ind, sz_data_ind * sizeof(char*));
            ASSERT(data_ind != NULL, NULL_MEMORY_ERR);
        }

        data_ind[*num_of_lines] = read_line(file_in);

        if (data_ind[*num_of_lines] != NULL) {
            (*num_of_lines)++;
        }
    }

    data_ind = (char**)realloc(data_ind, *num_of_lines * sizeof(char*));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    fclose(file_in);
    return data_ind;
}

char** read_file_by_lines_one_buf(const char* file_name, size_t* num_of_lines) {
    ASSERT(file_name != NULL, NULL_ERROR);
    ASSERT(num_of_lines != NULL, NULL_ERROR);

    int file_in = open(file_name, O_RDONLY);
    ASSERT(file_in != -1, NULL_CANT_OPEN_FILE);

    struct stat stat_file = {};
    fstat(file_in, &stat_file);

    *num_of_lines = 0;
    size_t sz_data = stat_file.st_size;
    char* data = (char*)calloc(sz_data + 1, sizeof(char));
    ASSERT(data != NULL, NULL_MEMORY_ERR);

    int sz_text = read(file_in, data, sz_data);
    data = (char*)realloc(data, (sz_text + 1) * sizeof(char));
    ASSERT(data != NULL, NULL_MEMORY_ERR);

    data[sz_text] = '\0';

    size_t sz_data_ind = 32;
    char** data_ind = (char**)calloc(sz_data_ind, sizeof(char*));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);
    char* last_ind = data;

    for (size_t i = 0; i <= sz_text; i++) {
        if (*num_of_lines == sz_data_ind) {
            sz_data_ind *= 2;
            data_ind = (char**)realloc(data_ind, sz_data_ind * sizeof(char*));
            ASSERT(data_ind != NULL, NULL_MEMORY_ERR);
        }

        if (data[i] == '\n' || data[i] == '\0') {
            data_ind[*num_of_lines] = last_ind;
            last_ind = data + i + 1;
            *num_of_lines += 1;
            data[i] = '\0';
        }
    }

    data_ind = (char**)realloc(data_ind, *num_of_lines * sizeof(char*));
    ASSERT(data_ind != NULL, NULL_MEMORY_ERR);

    close(file_in);
    return data_ind;
}

int compare_by_ptr(const void* ptr_a, const void* ptr_b) {
    return *(char**)ptr_a < *(char**)ptr_b;
}