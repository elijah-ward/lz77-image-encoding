#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void encode_using_lz77(
    char *in_filename_PGM_ptr,
    unsigned int searching_buffer_size,
    float *avg_offset_ptr,
    float *std_offset_ptr,
    float *avg_length_ptr,
    float *std_length_ptr );

typedef struct {
    int offset;
    int length;
    char mismatch_char;
} token;

