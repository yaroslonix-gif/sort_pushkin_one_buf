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
#include <time.h>


/////////////////////////////////////////////////////////// DEFINE ///////////////////////////////////////////////////////////
// - use for debug mode
#define DEBUG_ON


//============================================== COLOR ==============================================//
#define BLACK_TEXT       "\u001b[30m"
#define RED_TEXT         "\u001b[31m"
#define GREEN_TEXT       "\u001b[32m"
#define YELLOW_TEXT      "\u001b[33m"
#define BLUE_TEXT        "\u001b[34m"
#define PURPLE_TEXT      "\u001b[35m"
#define TURQUOISE_TEXT   "\u001b[36m"
#define WHITE_TEXT       "\u001b[37m"
#define RESET_COLOR_TEXT "\u001b[0m"


//============================================== CONST VALUES ==============================================//
#define BASE_INIT_ARRAY_LENGTH 32

#define lEN_OF_PRINT_LINE 150

#define LOGG_FILE_NAME "logg.log"


/////////////////////////////////////////////////////////// МАКРОС-ФУНКЦИИ ///////////////////////////////////////////////////////////

//============================================== LOGG ==============================================//
#define LOGG(...) fprintf(LOGG_F_USE_ONLY_FOR_LOGG, __VA_ARGS__)

#define LOGG_TIME { time_t TIMER = time(NULL); \
    struct tm *G_TIMER = localtime(&TIMER); \
    LOGG("%d:%d:%d", G_TIMER->tm_hour, G_TIMER->tm_min, G_TIMER->tm_sec); }

#define ALOGG { LOGG_TIME \
    LOGG(":%s:%d:%s\n", __FILE__, __LINE__, __func__); } //autologg

#define START_LOGG { \
    LOGG_F_USE_ONLY_FOR_LOGG = fopen(LOGG_FILE_NAME, "w"); \
    ASSERT(LOGG_F_USE_ONLY_FOR_LOGG != NULL, NULL_ERROR); \
    setvbuf(LOGG_F_USE_ONLY_FOR_LOGG, NULL, _IONBF, 0); \
    ALOGG; \
}

#define STOP_LOGG { \
    fclose(LOGG_F_USE_ONLY_FOR_LOGG); \
}


//============================================== DEBUG ==============================================//
#ifdef DEBUG_ON
    #define ASSERT(value, err) if ((value) == 0)\
        LOGG("%s%s:%d: ASSERTion error: %s%s\n", RED_TEXT, __FILE__, __LINE__, __STRINGIFY(err), RESET_COLOR_TEXT);
#else
    #define ASSERT(value, err) ;
#endif


//============================================== PRINT ==============================================//
#define PRINT_COLOR(color, ...) { \
    printf(color __VA_ARGS__); \
    printf(RESET_COLOR_TEXT); \
}


//============================================== BASE OPERATIONS ==============================================//
#define SWAP(A, B, type) { type tmp = A; A = B; B = tmp; }
#define MIN(A, B) ((A < B) ? (A) : (B))
#define MAX(A, B) ((A > B) ? (A) : (B))


/////////////////////////////////////////////////////////// ENUM ///////////////////////////////////////////////////////////
enum error {
    NULL_ERROR = 0,
    NULL_MEMORY_ERR = 1,
    NULL_CANT_OPEN_FILE = 2,
};

enum compare {
    LESS_COMPARE  = -1,
    MORE_COMPARE  =  1,
    EQUAL_COMPARE =  0
};


/////////////////////////////////////////////////////////// STRUCT ///////////////////////////////////////////////////////////
struct String{
    char* str;
    size_t len;
};


/////////////////////////////////////////////////////////// GLOBAL ///////////////////////////////////////////////////////////
extern FILE *LOGG_F_USE_ONLY_FOR_LOGG; // ИСПОЛЬЗОВАТЬ ТОЛЬКО ПО НАЗНАЧЕНИЮ


/////////////////////////////////////////////////////////// ПРОТОТИПЫ ФУНКЦИЙ ///////////////////////////////////////////////////////////
void universal_swap(const void* restrict ptr_a, const void* restrict ptr_b, size_t elem_sz);

void print_line(FILE* out_stream);

void print_line_color(FILE* out_stream, const char* color);

size_t get_file_size(int file);