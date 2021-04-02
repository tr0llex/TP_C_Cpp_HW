#include "gtest/gtest.h"

extern "C" {
#include "numseq.h"
#include <dlfcn.h>
}

TEST(stress_and_comparison_test, test) {
    FILE *file = fopen("../../tests/test100mb.txt", "r");
    struct stat file_info;
    fstat(fileno(file), &file_info);
    size_t filesize = file_info.st_size;
    char *arr = (char*) malloc(sizeof(char) * filesize);
    for (size_t i = 0; i < filesize; i++) {
        char buffer = '\0';
        fscanf(file, "%c", &buffer);
        arr[i] = buffer;
    }
    fclose(file);

    void *library = 0;
    int (*max_number_sequence)(const char *arr, subsequence *subseq_max, const size_t arr_length);
    library = dlopen("../lib_num_sequence_parallel.so", RTLD_NOW);
    *(void **) (&max_number_sequence) = dlsym(library, "max_number_sequence");

    size_t arr_length = 15;
    subsequence subseq_max_p = {0};
    subsequence subseq_max_c = {0};

    (*max_number_sequence)(arr, &subseq_max_p, arr_length);
    max_number_sequence(arr, &subseq_max_c, arr_length);

    ASSERT_EQ(subseq_max_c.start, subseq_max_p.start);
    ASSERT_EQ(subseq_max_c.end, subseq_max_p.end);
    ASSERT_EQ(subseq_max_c.len, subseq_max_p.len);
}
