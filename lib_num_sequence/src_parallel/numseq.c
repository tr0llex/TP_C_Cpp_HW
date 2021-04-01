#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include "numseq.h"

int create_ranges(int count, size_t filesize, size_t *ranges) {
    size_t proc_range = filesize / count;

    if (ranges == NULL) {
        return CODE_ERROR;
    }
    for (int i = 1; i < count; i++) {
        ranges[i] = ranges[i - 1] + proc_range;
    }
    ranges[count] = filesize;
    return 0;
}

int fork_calculations(int *processes, int process_count) {
    if (processes == NULL) {
        return CODE_ERROR;
    }
    for (int i = 0; i < process_count; i++) {
        int current_id = fork();
        if (current_id == -1) {
            return CODE_ERROR;
        }
        if (current_id == 0) {
            return i;
        }
        processes[i] = current_id;
    }
    return getpid();
}

void free_ranges_and_processes(size_t *ranges, int *processes) {
    if (ranges) {
        free(ranges);
    }
    if (processes) {
        free(processes);
    }
}

int max_number_sequence(const char *arr, subsequence *subseq_max, size_t arr_length) {
    if (arr == NULL || subseq_max == NULL) {
        return CODE_ERROR;
    }

    size_t process_count = (arr_length > 100) ? sysconf(_SC_NPROCESSORS_ONLN) : 2;

    size_t *ranges = (size_t *) calloc(process_count + 1, sizeof(size_t));
    create_ranges(process_count, arr_length, ranges);

    int pipes[MAX_PROCESS_COUNT][2];
    memset(pipes, 0, MAX_PROCESS_COUNT * 2);

    for (int i = 0; i < process_count; i++) {
        pipe(pipes[i]);
    }

    int *processes = (int *) calloc(process_count, sizeof(pid_t));
    int current_id = fork_calculations(processes, process_count);

    // CHILD PROCESS
    if (getpid() != current_id) {
        close(pipes[current_id][0]);

        subsequence subseq_cur = {0, 0, 0};
        subsequence subseq_max_child = {0, 0, 0};
        subsequence subseq_first = {0, 0, 0};
        subsequence subseq_last = {0, 0, 0};

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
                    subseq_last.len = subseq_cur.len;
                    subseq_last.start = subseq_cur.start;
                }
                subseq_cur.len = 0;
                subseq_cur.start = 0;
            }
            subseq_max_child.end = subseq_max_child.start + subseq_max_child.len;
            subseq_first.end = subseq_first.start + subseq_first.len;
            subseq_last.end = subseq_last.start + subseq_last.len;
        }

        write(pipes[current_id][1], &subseq_max_child, sizeof(subsequence));
        write(pipes[current_id][1], &subseq_first, sizeof(subsequence));
        write(pipes[current_id][1], &subseq_last, sizeof(subsequence));

        close(pipes[current_id][1]);
        free_ranges_and_processes(ranges, processes);
        exit(0);
    }

    // Структура для хранения подпоследовательностей, лежащих на нескольких отрезках
    subsequence subseq_global = {0, 0, 0};

    // PARENT PROCESS
    for (int i = 0; i < process_count; i++) {
        close(pipes[i][1]);

        if (waitpid(processes[i], NULL, 0) == -1) {
            return CODE_ERROR;
        }

        subsequence subseq_child_max = {0, 0, 0};
        read(pipes[i][0], &subseq_child_max, sizeof(subsequence));

        if (subseq_child_max.len > subseq_max->len) {
            subseq_max->start = subseq_child_max.start;
            subseq_max->end = subseq_child_max.end;
            subseq_max->len = subseq_child_max.len;
        }

        subsequence subseq_child_first = {0, 0, 0};
        read(pipes[i][0], &subseq_child_first, sizeof(subsequence));

        subsequence subseq_child_last = {0, 0, 0};
        read(pipes[i][0], &subseq_child_last, sizeof(subsequence));


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

    free_ranges_and_processes(ranges, processes);
    return CODE_SUCCESS;
}
