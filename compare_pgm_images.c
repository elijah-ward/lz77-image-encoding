#include <stdio.h>
#include <stdlib.h>
#include "mean_absolute_error.h"

int validate_args(int argc) {
    if (argc != 3) {
        printf("Usage: ./compare_pgm_images [input pgm image filename 1] [input pgm image filename 2]\nError on argument validation. Exiting...\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    int status = validate_args(argc);
    if (status == 1) {
        exit(0);
    }

    char *input_pgm_filename_1 = argv[2];
    char *input_pgm_filename_2 = argv[3];

    mean_absolute_error(input_pgm_filename_1, input_pgm_filename_2);

}
