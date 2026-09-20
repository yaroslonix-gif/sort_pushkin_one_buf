#include "base.h"



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

void print_line(FILE* out_stream) {
    ASSERT(out_stream != NULL, NULL_ERROR);

    for (int i = 0; i < lEN_OF_PRINT_LINE; i++)
        fprintf(out_stream, "=");
    fprintf(out_stream, "\n");
}

void print_line_color(FILE* out_stream, const char* color) {
    ASSERT(out_stream != NULL, NULL_ERROR);
    ASSERT(color != NULL, NULL_ERROR);

    fprintf(out_stream, color);
    print_line(out_stream);
    fprintf(out_stream, "%s", RESET_COLOR_TEXT);
}