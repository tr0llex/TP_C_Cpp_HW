#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include <stddef.h>

typedef struct {
    char *string;
    size_t count;
    size_t capacity;
} string_t;

string_t *init_string();
int enlarge_string(string_t *);
int add_char(string_t *, char);
int free_string(string_t *);

#endif  // INCLUDE_MY_STRING_H_
