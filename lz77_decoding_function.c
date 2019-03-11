#include "lz77_decoding_function.h"

/**
 * @brief      { decodes a lz77 encoded file to original structure }
 *
 * @param      in_compressed_filename_ptr  In compressed filename pointer
 */

void decode_using_lz77(char *in_compressed_filename_ptr) {

    /* ============== Encoded FILE Input =============== */

    FILE *in_fp;
    in_fp = fopen(in_compressed_filename_ptr, "r");
    if (!in_fp) perror("Input file failed to open!"), exit(1);

    unsigned int searching_buffer_size = 0;
    int n_tokens = 0;
    long f_size = 0;

    fscanf(in_fp, "%u\n%d\n%li\n", &searching_buffer_size, &n_tokens, &f_size);

    int offsets[n_tokens+1];
    int lengths[n_tokens+1];
    char mismatches[n_tokens+1];

    // Read in offset, length, and mismatch arrays

    fread(offsets, sizeof(int), n_tokens, in_fp);
    fread(lengths, sizeof(int), n_tokens, in_fp);
    fread(mismatches, sizeof(char), n_tokens, in_fp);
    fclose(in_fp);

    /* ============ DECODE =========== */

    char *data = calloc( f_size + 1, 1 );
    char *buffer_start = NULL, *buffer_end = NULL, *match_start = NULL, *match_end = NULL;
    int buffer_size = 0;

    // Initialize the sliding window of our buffer
    buffer_start = data;
    buffer_end = data + buffer_size;

    for ( int i = 0; i <= n_tokens; i++ ) {

        // Gather the offset, length, mismatch of the first token
        int offset = offsets[i];
        int length = lengths[i];
        int mismatch = mismatches[i];

        // If this token has a match, gather the characters from the match
        if ( offset != 0 ) {

            char *match = NULL;

            match_start = buffer_end - offset;
            match_end = match_start + length;

            match = match_start;

            // append all characters from the match to the buffer
            while (match != match_end) {
                *(buffer_end + (match - match_start)) = *match;
                match++;
            }

            buffer_end += length;

        }

        *buffer_end = mismatch;
        buffer_end = buffer_end + 1;

        // Our buffer must always be constrained to the searching_buffer_size so
        // if we have had a match we must shift/extend by the length of the match
        // else if we had no match we must account for the mismatch character that is added
        // in each case if we have hit the searching_buffer_size then we shift the input data pointer to slide the window

        if (length > 0) {
            int shift = length + 1;
            if (buffer_size + shift <= searching_buffer_size) buffer_size += shift; else buffer_start += shift;
        } else {
            int shift = 1;
            if (buffer_size < searching_buffer_size) buffer_size += shift; else buffer_start += shift;
        }
    }

    /* ========= Decoded FILE Output ============ */

    char *out_filename = strcat( in_compressed_filename_ptr, ".pgm");

    FILE *out_fp;
    out_fp = fopen(out_filename, "w+");
    if (!out_fp) perror("Output file failed to open!"), exit(1);

    fwrite(data, sizeof(char), f_size, out_fp);
    fclose(out_fp);

}