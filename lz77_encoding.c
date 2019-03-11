#include "lz77_encoding_function.h"

/**
 * @brief      { validates arguments }
 *
 * @param[in]  argc  The argc
 *
 * @return     { status }
 */

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

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    float avg_offset, std_offset, avg_length, std_length;
    char *input_pgm_filename = argv[1];
    char *sbs_ptr;
    unsigned int search_buffer_size = strtoul(argv[2], &sbs_ptr, 10);

    encode_using_lz77(input_pgm_filename, search_buffer_size, &avg_offset, &std_offset, &avg_length, &std_length);

    end = clock();
    cpu_time_used = ((double) (end - start) / CLOCKS_PER_SEC);

    printf("Encoding Complete!\nElapsed time: %lf\nsearch_buffer_size: %u\navg_offset: %f\nstd_offset: %f\navg_length: %f\nstd_length: %f\n",
        cpu_time_used, search_buffer_size, avg_offset, std_offset, avg_length, std_length);

}