#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEBUG_ON
// - use for debug mode

#define RED         "\u001b[31m"
#define GREEN       "\u001b[32m"
#define YELLOW      "\u001b[33m"
#define BLUE        "\u001b[34m"
#define TURQUOISE   "\u001b[36m"
#define RESET_COLOR "\u001b[0m"


#define lEN_OF_PRINT_LINE 150

#define LOGG(...) fprintf(stderr, __VA_ARGS__)

#ifdef DEBUG_ON
    #define ASSERT(value, err) if ((value) == 0)\
        LOGG("%s%s:%d: ASSERTion error: %s%s\n", RED, __FILE__, __LINE__, __STRINGIFY(err), RESET_COLOR);
    #define PRINT_QSORT_DEBUG(S_REASON) debug_qsort_print(data, left, right, sz, S_REASON);
#else
    #define ASSERT(value, err) ;
    #define PRINT_QSORT_DEBUG(S_REASON) ;
#endif

#define PRINT_COLOR(color, ...) {printf(color __VA_ARGS__); printf(RESET_COLOR);}

#define SWAP(A, B, type) {type tmp = A; A = B; B = tmp;}
#define MIN(A, B) ((A < B) ? (A) : (B))
#define MAX(A, B) ((A > B) ? (A) : (B))


enum error {
    NULL_ERROR = 0,
    NULL_MEMORY_ERR = 1,
    NULL_CANT_OPEN_FILE = 2,
};