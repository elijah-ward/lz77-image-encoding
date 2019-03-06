#include <stdio.h>
#include <stdlib.h>
#include "lz77_encoding_function.h"

int validate_args(int argc) {
    if (argc != 3) {
        printf("Usage: ./lz77_encoding [input pgm image filename] [searching buffer size]\nError on argument validation. Exiting...\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    int status = validate_args(argc);
    if (status == 1) {
        exit(0);
    }

    float avg_offset, std_offset, avg_length, std_length;
    char *input_pgm_filename = argv[2];
    unsigned int search_buffer_size = (unsigned int) argv[3];

    encode_using_lz77(input_pgm_filename, search_buffer_size, &avg_offset, &std_offset, &avg_length, &std_length);

}