#ifndef INCLUDE_MY_STRING_H_
#define INCLUDE_MY_STRING_H_

#include <stddef.h>

#define CODE_ERROR 1
#define CODE_SUCCESS 0

typedef struct {
    char *string;
    size_t count;
    size_t capacity;
} string_t;

string_t *init_string();
int add_char(string_t *str, char symbol);
int free_string(string_t *str);

#endif  // INCLUDE_MY_STRING_H_
