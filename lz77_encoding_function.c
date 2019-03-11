#include "lz77_encoding_function.h"

#define MAX_TOKENS 100000

/**
 * @brief      { encodes a file to lz77 format }
 *
 * @param      in_filename_PGM_ptr    The filename of PGM to be encoded
 * @param[in]  searching_buffer_size  The searching buffer size
 * @param      avg_offset_ptr         The average offset pointer
 * @param      std_offset_ptr         The standard offset pointer
 * @param      avg_length_ptr         The average length pointer
 * @param      std_length_ptr         The standard length pointer
 */

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