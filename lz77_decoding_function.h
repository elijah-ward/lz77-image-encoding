#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void decode_using_lz77(char *in_compressed_filename_ptr);

typedef struct {
    int offset;
    int length;
    char mismatch_char;
} token;