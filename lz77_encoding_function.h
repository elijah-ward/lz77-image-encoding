#include <stdio.h>
#include <stdlib.h>

void encode_using_lz77(
    char *in_filename_PGM_ptr,
    unsigned int searching_buffer_size,
    float *avg_offset_ptr,
    float *std_offset_ptr,
    float *avg_length_ptr,
    float *std_length_ptr );