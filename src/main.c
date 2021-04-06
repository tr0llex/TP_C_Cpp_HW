#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "numseq.h"

#define TESTS_COUNT 5

int load_file_to_arr(FILE *file, char *arr, size_t count) {
    if (file == NULL || arr == NULL) {
        return CODE_ERROR;
    }

    for (int i = 0; i < count; i++) {
        char buffer = '\0';
        if (!fscanf(file, "%c", &buffer)) {
            return CODE_ERROR;
        }
        arr[i] = buffer;
    }

    if (fclose(file) != CODE_SUCCESS) {
        return CODE_ERROR;
    }

    return CODE_SUCCESS;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        return CODE_ERROR;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        return CODE_ERROR;
    }

    struct stat file_info;
    fstat(fileno(file), &file_info);
    if ((file_info.st_mode & S_IRUSR) == 0 || file_info.st_size == 0) {
        return CODE_ERROR;
    }

    long l1dcls = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    if (l1dcls == -1) {
        l1dcls = sizeof(void *);
    }
    char *arr_chars;
    if (posix_memalign((void **) &arr_chars, l1dcls, sizeof(char) * file_info.st_size)) {
        return CODE_ERROR;
    }

    if (load_file_to_arr(file, arr_chars, file_info.st_size) == CODE_ERROR) {
        return CODE_ERROR;
    }


    double elapsed[TESTS_COUNT] = {0};
    double average[2] = {0};  // [0] for consistent, [1] for parallel
    subsequence subseq_max_c = {0, 0, 0};
    subsequence subseq_max_p = {0, 0, 0};

    for (int i = 0; i < TESTS_COUNT; i++) {
        struct timespec start, finish;
        subseq_max_c = (const subsequence) {0};
        if (clock_gettime(_POSIX_MONOTONIC_CLOCK, &start) == -1) {
            return CODE_ERROR;
        }
        if (max_number_sequence(arr_chars, &subseq_max_c, file_info.st_size) == CODE_ERROR) {
            return CODE_ERROR;
        }
        if (clock_gettime(_POSIX_MONOTONIC_CLOCK, &finish) == -1) {
            return CODE_ERROR;
        }

        elapsed[i] = (double) (finish.tv_sec - start.tv_sec);
        elapsed[i] += (double) (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        average[0] += elapsed[i];  // add to consistent

        void *library = 0;
        int (*max_number_sequence)(const char *arr, subsequence *subseq_max, const size_t arr_length);
        library = dlopen("./lib_num_sequence_parallel.so", RTLD_NOW);
        if (!library) {
            printf("%s", "Library not found");
            return CODE_ERROR;
        }
        *(void **) (&max_number_sequence) = dlsym(library, "max_number_sequence");
        subseq_max_p = (const subsequence) {0};
        if (clock_gettime(_POSIX_MONOTONIC_CLOCK, &start) == -1) {
            return CODE_ERROR;
        }
        if ((*max_number_sequence)(arr_chars, &subseq_max_p, file_info.st_size) == CODE_ERROR) {
            return CODE_ERROR;
        }
        if (clock_gettime(_POSIX_MONOTONIC_CLOCK, &finish) == -1) {
            return CODE_ERROR;
        }
        dlclose(library);
        elapsed[i] = (double) (finish.tv_sec - start.tv_sec);
        elapsed[i] += (double) (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        average[1] += elapsed[i];  // add to parallel
    }

    if (subseq_max_c.start == subseq_max_p.start && subseq_max_c.end == subseq_max_p.end &&
        subseq_max_c.len == subseq_max_p.len) {
        printf("Answer - start: %lu, end: %lu, len: %lu\n", subseq_max_c.start, subseq_max_c.end, subseq_max_c.len);
    } else {
        printf("Consistent and parallel implementation have different answers!\n");
    }
    average[0] = average[0] / TESTS_COUNT;
    average[1] = average[1] / TESTS_COUNT;
    printf("Consistent series average time:  %lf sec\n", average[0]);
    printf("Parallel series average time:    %lf sec\n", average[1]);

    free(arr_chars);
    return CODE_SUCCESS;
}
