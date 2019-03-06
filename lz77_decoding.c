#include <stdio.h>
#include <stdlib.h>
#include "lz77_decoding_function.h"

int validate_args(int argc) {
    if (argc != 2) {
        printf("Usage: ./lz77_decoding [output pgm image filename]\nError on argument validation. Exiting...\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    int status = validate_args(argc);
    if (status == 1) {
        exit(0);
    }

    char *output_pgm_filename = argv[2];
    decode_using_lz77(output_pgm_filename);

}