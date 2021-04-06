#ifndef LIB_NUM_SEQUENCE_INCLUDE_NUMSEQ_H_
#define LIB_NUM_SEQUENCE_INCLUDE_NUMSEQ_H_

#define CODE_ERROR 1
#define CODE_SUCCESS 0

typedef struct subsequence {
    size_t start;
    size_t end;
    size_t len;
} subsequence;

int max_number_sequence(const char *arr, subsequence *subseq_max, const size_t arr_length);

#endif  // LIB_NUM_SEQUENCE_INCLUDE_NUMSEQ_H_
