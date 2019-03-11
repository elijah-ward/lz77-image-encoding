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

/* ============= INPUT File Read ============= */

    FILE *in_fp;
    long f_size;
    char *data;
    in_fp = fopen(in_filename_PGM_ptr, "r");
    if (!in_fp) perror("Input file failed to open!"), exit(1);
    fseek(in_fp, 0L, SEEK_END);
    f_size = ftell(in_fp);
    rewind(in_fp);

    data = calloc(1, f_size + 1);
    fread(data, f_size, 1, in_fp);
    fclose(in_fp);

/* ============ ENCODE DATA ================== */

    char *data_start = data;
    char *data_end = data + f_size;
    token *tokens = (token *) malloc(MAX_TOKENS * sizeof(token));
    token *token_start = tokens;
    char *buffer_start = NULL, *buffer_end = NULL, *buffer_match = NULL, *data_match = NULL;
    int buffer_size = 0;

    while ((data + buffer_size) <= data_end) {

        // Initialize token to hold the next available char in case we find no matches
        char next_char = *(data + buffer_size);
        token tok = {0, 0, next_char};

        // Shift buffer over the current region of input data
        buffer_start = data;
        buffer_end = data + buffer_size;

        // Start looking for matches at the beginning of the buffer
        char *match_scanner = buffer_start;

        // While we are not at the end of the buffer
        while ( match_scanner != buffer_end ) {

            // If we find a char equal to the next_char being considered, start finding the longest match
            if ( *(match_scanner) == next_char) {

                char *match = match_scanner;
                // Scan the consecutive chars after the first as long as we aren't at the end of the buffer
                while (match != buffer_end) {
                    buffer_match = match;
                    data_match = buffer_end;

                    // If the match has ended, break the loop
                    if ( *buffer_match != *data_match )
                        break;

                    // Iterate through the buffer to find more matches
                    while (1) {
                        buffer_match++;
                        data_match++;
                        if (!(*buffer_match == *data_match) || data_match == data_end ) break;
                    }

                    // prepare values for the current longest match
                    int m_length = buffer_match - match;
                    int m_offset = buffer_end - match;
                    char m_char = *data_match;

                    // If the current match is longer than the one we had, assign the token its values
                    if ( m_length >= tok.length ) {
                        tok.length = m_length;
                        tok.offset = m_offset;
                        tok.mismatch_char = m_char;
                    }
                    match++;
                }
            }
            match_scanner++;
        }

        *tokens++ = tok;

        // Our buffer must always be constrained to the searching_buffer_size so
        // if we have had a match we must shift/extend by the length of the match
        // else if we had no match we must account for the mismatch character that is added
        // in each case if we have hit the searching_buffer_size then we shift the input data pointer to slide the window

        if (tok.length > 0) {
            int shift = tok.length + 1;
            if (buffer_size + tok.length + 1 <= searching_buffer_size) buffer_size += shift; else data += shift;
        } else {
            int shift = 1;
            if (buffer_size < searching_buffer_size) buffer_size += shift; else data += shift;
        }
    }
    free(data_start);

/* ============== Collect Offsets, Lengths, Mismatches, Statistics ============= */

    int n_tokens = tokens - token_start;

    int offsets[n_tokens+1];
    int max_offset = 0;

    int lengths[n_tokens+1];
    int max_length = 0;

    char mismatches[n_tokens+1];

    int offset_sum = 0;
    int length_sum = 0;

    for (int i = 0; i < n_tokens; i++) {
        token curr_token = *(token_start + i);

        offsets[i] = curr_token.offset;
        offset_sum += curr_token.offset;

        lengths[i] = curr_token.length;
        length_sum += curr_token.length;

        mismatches[i] = curr_token.mismatch_char;

        if (curr_token.offset >= max_offset) max_offset = curr_token.offset;
        if (curr_token.length >= max_length) max_length = curr_token.length;
    }

    float offset_mean = (float) offset_sum / (float) n_tokens;
    float length_mean = (float) length_sum / (float) n_tokens;


    float offset_accumulator = 0;
    float length_accumulator = 0;

    for (int i = 0; i < n_tokens; i++) {
        offset_accumulator += powf( ((float) offsets[i] - offset_mean), 2);
        length_accumulator += powf( ((float) lengths[i] - length_mean), 2);
    }

    float offset_stddev = sqrtf(offset_accumulator / (float) n_tokens);
    float length_stddev = sqrtf(length_accumulator / (float) n_tokens);

    *avg_offset_ptr = offset_mean;
    *std_offset_ptr = offset_stddev;
    *avg_length_ptr = length_mean;
    *std_length_ptr = length_stddev;

    free(token_start);

    /* ========== OUTPUT Encoded File =========== */

    char buff_size[10];
    snprintf(buff_size, 10, "%d", searching_buffer_size);

    char out_name_buff[50];
    strcpy(out_name_buff, in_filename_PGM_ptr);
    char *out_filename = strcat( strcat( strcat(out_name_buff, "."), buff_size), ".lz");

    FILE *out_fp;
    out_fp = fopen(out_filename, "w+");
    if (!out_fp) perror("Output file failed to write!"), exit(1);

    fprintf(out_fp,"%u\n%d\n%li\n", searching_buffer_size, n_tokens, f_size);
    fwrite(offsets, sizeof(int), n_tokens, out_fp);
    fwrite(lengths, sizeof(int), n_tokens, out_fp);
    fwrite(mismatches, sizeof(char), n_tokens, out_fp);

    fclose(out_fp);

    /* ===== Count Offset & Length Frequencies ====== */

    int *offset_frequency = (int*) calloc(max_offset+1, sizeof(int));

    for ( int i = 0; i < n_tokens; i++ ) {
        *(offset_frequency + offsets[i]) += 1;
    }

    /* ========== OUTPUT Offsets CSV File =========== */

    char off_name_buff[50];
    strcpy(off_name_buff, in_filename_PGM_ptr);
    char *offset_filename = strcat( strcat( strcat(off_name_buff, "."), buff_size), ".offsets.csv");

    FILE *offset_fp;
    offset_fp = fopen(offset_filename, "w+");
    if (!offset_fp) perror("Output file failed to write!"), exit(1);

    for (int i = 0; i < max_offset + 1; i++) {
        fprintf(offset_fp, "%d, %d\n", i, *(offset_frequency + i));
    }

    free(offset_frequency);
    fclose(offset_fp);

    /* ========== OUTPUT Lengths CSV File =========== */

    int *length_frequency = (int*) calloc(max_length+1, sizeof(int));

    for ( int i = 0; i < n_tokens; i++ ) {
        *(length_frequency + lengths[i]) += 1;
    }

    char len_name_buff[50];
    strcpy(len_name_buff, in_filename_PGM_ptr);
    char *length_filename = strcat( strcat( strcat(len_name_buff, "."), buff_size), ".lengths.csv");

    FILE *length_fp;
    length_fp = fopen(length_filename, "w+");
    if (!length_fp) perror("Output file failed to write!"), exit(1);

    for (int i = 0; i < max_length + 1; i++) {
        fprintf(length_fp, "%d, %d\n", i, *(length_frequency + i));
    }

    free(length_frequency);
    fclose(length_fp);
}