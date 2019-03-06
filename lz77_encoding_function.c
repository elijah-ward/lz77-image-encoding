#include "lz77_encoding_function.h"

void encode_using_lz77(
    char *in_filename_PGM_ptr,
    unsigned int searching_buffer_size,
    float *avg_offset_ptr,
    float *std_offset_ptr,
    float *avg_length_ptr,
    float *std_length_ptr )
{
printf("INSIDE encode_using_lz77\n");

*avg_offset_ptr = 1;
*std_offset_ptr = 2;
*avg_length_ptr = 3;
*std_length_ptr = 4;

printf("in_filename_PGM_ptr[0]: %c\nsearching_buffer_size: %u\navg_offset_ptr: %f\nstd_offset_ptr: %f\navg_length_ptr: %f\nstd_length_ptr: %f\n",
    in_filename_PGM_ptr[0],
    searching_buffer_size,
    *avg_offset_ptr,
    *std_offset_ptr,
    *avg_length_ptr,
    *std_length_ptr);
}