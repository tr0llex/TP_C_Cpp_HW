#include <stddef.h>
#include "numseq.h"

int max_number_sequence(const char *arr, subsequence *subseq_max, const size_t arr_length) {
    if (arr == NULL || subseq_max == NULL) {
        return CODE_ERROR;
    }

    subsequence subseq_cur = {0, 0, 0};
    for (size_t i = 0; i < arr_length; i++) {
        char ch = arr[i];
        if ((ch >= '0') && (ch <= '9')) {
            if (subseq_cur.len == 0) {
                subseq_cur.start = i;
            }
            subseq_cur.len++;
            if (subseq_cur.len > subseq_max->len) {
                subseq_max->start = subseq_cur.start;
                subseq_max->end = subseq_cur.start + subseq_cur.len;
                subseq_max->len = subseq_cur.len;
            }
        } else {
            subseq_cur.len = 0;
        }
    }
    return CODE_SUCCESS;
}

