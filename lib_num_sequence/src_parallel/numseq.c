#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include "numseq.h"

int create_ranges(const int process_count, const size_t filesize, size_t *ranges) {
    if (ranges == NULL) {
        return CODE_ERROR;
    }
    size_t proc_range = filesize / process_count;
    for (int i = 1; i < process_count; i++) {
        ranges[i] = ranges[i - 1] + proc_range;
    }
    ranges[process_count] = filesize;
    return CODE_SUCCESS;
}

int fork_and_get_id(int *processes, const int process_count) {
    if (processes == NULL) {
        return -1;
    }
    for (int i = 0; i < process_count; i++) {
        int current_id = fork();
        if (current_id == -1) {
            return -1;
        }
        if (current_id == 0) {
            return i;
        }
        processes[i] = current_id;
    }
    return getpid();
}

void free_ranges_and_processes(size_t *ranges, int *processes, int **pipes, int pipes_count) {
    if (ranges) {
        free(ranges);
    }
    if (processes) {
        free(processes);
    }
    if (pipes) {
        for (int i = 0; i < pipes_count; i++) {
            if (pipes[i]) {
                free(pipes[i]);
            }
        }
        free(pipes);
    }
}

int child_process(const char *arr, int current_id, const size_t *ranges, int **pipes) {
    if (arr == NULL || ranges == NULL || pipes == NULL) {
        return CODE_ERROR;
    }

    subsequence subseq_cur = {0};  // Structure for storing current subsequence on current range
    subsequence subseq_max_child = {0};  // Structure for storing maximum subsequence on current range
    subsequence subseq_first = {0};  // Structure for storing first subsequence on current range
    subsequence subseq_last = {0};  // Structure for storing last subsequence on current range

    for (size_t i = ranges[current_id]; i < ranges[current_id + 1]; i++) {
        char ch = arr[i];
        if ((ch >= '0') && (ch <= '9')) {
            if (subseq_cur.len == 0) {
                subseq_cur.start = i;
                subseq_last.start = i;
                subseq_last.len = 0;
            }
            subseq_cur.len++;
            subseq_last.len++;
            if (subseq_cur.len > subseq_max_child.len) {
                subseq_max_child.start = subseq_cur.start;
                subseq_max_child.len = subseq_cur.len;
            }
            if (subseq_first.len == 0) {
                subseq_first.start = subseq_cur.start;
                subseq_first.len = subseq_cur.len;
            }
            if (subseq_first.start == subseq_cur.start) {
                subseq_first.len = subseq_cur.len;
            }
        } else {
            if (subseq_cur.len > 0) {
                subseq_last.start = subseq_cur.start;
                subseq_last.len = subseq_cur.len;
            }
            subseq_cur.start = 0;
            subseq_cur.len = 0;
        }
        subseq_max_child.end = subseq_max_child.start + subseq_max_child.len;
        subseq_first.end = subseq_first.start + subseq_first.len;
        subseq_last.end = subseq_last.start + subseq_last.len;
    }

    write(pipes[current_id][1], &subseq_max_child, sizeof(subsequence));
    write(pipes[current_id][1], &subseq_first, sizeof(subsequence));
    write(pipes[current_id][1], &subseq_last, sizeof(subsequence));

    close(pipes[current_id][1]);
    return CODE_SUCCESS;
}

int parent_process(int **pipes, int process_count, subsequence *subseq_max, const size_t *ranges, int *processes) {
    if (pipes == NULL || subseq_max == NULL || ranges == NULL || processes == NULL) {
        return CODE_ERROR;
    }

    subsequence subseq_global = {0};  // Structure for storing subsequences on several ranges at once
    for (int i = 0; i < process_count; i++) {
        close(pipes[i][1]);

        if (waitpid(processes[i], NULL, 0) == -1) {
            return CODE_ERROR;
        }

        subsequence subseq_child_max = {0};
        subsequence subseq_child_first = {0};
        subsequence subseq_child_last = {0};
        read(pipes[i][0], &subseq_child_max, sizeof(subsequence));
        read(pipes[i][0], &subseq_child_first, sizeof(subsequence));
        read(pipes[i][0], &subseq_child_last, sizeof(subsequence));

        if (subseq_child_max.len > subseq_max->len) {
            subseq_max->start = subseq_child_max.start;
            subseq_max->end = subseq_child_max.end;
            subseq_max->len = subseq_child_max.len;
        }

        if (subseq_global.len > 0) {
            if (subseq_child_first.start == ranges[i]) {
                subseq_global.len += subseq_child_first.len;
                subseq_global.end = subseq_child_first.end;
            }
            if (subseq_global.len > subseq_max->len) {
                subseq_max->start = subseq_global.start;
                subseq_max->end = subseq_global.end;
                subseq_max->len = subseq_global.len;
            }
            if (subseq_child_first.start != ranges[i]) {
                subseq_global = (const subsequence) {0};
            }
        } else if (subseq_global.len == 0 && subseq_child_last.end == ranges[i + 1]) {
            subseq_global.start = subseq_child_last.start;
            subseq_global.end = subseq_child_last.end;
            subseq_global.len = subseq_child_last.len;
        }
        close(pipes[i][0]);
    }
    return CODE_SUCCESS;
}

int max_number_sequence(const char *arr, subsequence *subseq_max, const size_t arr_length) {
    if (arr == NULL || subseq_max == NULL) {
        return CODE_ERROR;
    }

    size_t process_count = sysconf(_SC_NPROCESSORS_ONLN);

    size_t *ranges = (size_t *) calloc(process_count + 1, sizeof(size_t));
    if (ranges == NULL) {
        return CODE_ERROR;
    }
    if (create_ranges(process_count, arr_length, ranges) == CODE_ERROR) {
        return CODE_ERROR;
    }

    int **pipes = (int **) calloc(process_count, sizeof(int *));
    if (pipes == NULL) {
        return CODE_ERROR;
    }
    for (int i = 0; i < process_count; i++) {
        pipes[i] = (int *) calloc(2, sizeof(int));  // 2 because each process has only 2 pipes: for read, for write
        if (pipes[i] == NULL) {
            return CODE_ERROR;
        }
    }

    for (int i = 0; i < process_count; i++) {
        pipe(pipes[i]);
    }

    int *processes = (int *) calloc(process_count, sizeof(pid_t));
    if (processes == NULL) {
        return CODE_ERROR;
    }
    int current_id = fork_and_get_id(processes, process_count);
    if (current_id == -1) {
        return CODE_ERROR;
    }

    if (getpid() != current_id) {
        close(pipes[current_id][0]);
        if (child_process(arr, current_id, ranges, pipes) == CODE_ERROR) {
            return CODE_ERROR;
        }
        free_ranges_and_processes(ranges, processes, pipes, process_count);
        exit(0);
    }

    if (parent_process(pipes, process_count, subseq_max, ranges, processes) == CODE_ERROR) {
        return CODE_ERROR;
    }
    free_ranges_and_processes(ranges, processes, pipes, process_count);
    return CODE_SUCCESS;
}
