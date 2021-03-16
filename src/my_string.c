#include "my_string.h"
#include <stdlib.h>

#define CODE_ERROR 1
#define CODE_SUCCESS 0

string_t *init_string() {
    string_t *new_str = malloc(sizeof(string_t));
    if (new_str == NULL) {
        free_string(new_str);
        return NULL;
    }
    new_str->string = calloc(2, sizeof(char));  // using calloc() to init with '\0'
    if (new_str->string == NULL) {
        free_string(new_str);
        return NULL;
    }
    new_str->capacity = 2;
    new_str->count = 0;
    return new_str;
}

int enlarge_string(string_t *str) {
    if (str == NULL) {
        return CODE_ERROR;
    }

    char *temp = (char *) realloc(str->string, str->capacity * 2 * sizeof(char));
    if (temp == NULL) {
        free(temp);
        return CODE_ERROR;
    }

    str->string = temp;
    str->capacity *= 2;
    // free(temp);
    return CODE_SUCCESS;
}

int add_char(string_t *str, char symbol) {
    if (str == NULL) {
        return CODE_ERROR;
    }

    if (str->count == str->capacity - 1) {  // minus 1 because of '\0' symbol
        if (enlarge_string(str) != CODE_SUCCESS) {
            return CODE_ERROR;
        }
    }

    str->string[str->count] = symbol;
    str->count++;

    return CODE_SUCCESS;
}

int free_string(string_t *str) {
    if (str == NULL) {
        return CODE_ERROR;
    }

    free(str->string);
    free(str);
    return CODE_SUCCESS;
}
