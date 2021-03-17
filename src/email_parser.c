#include <malloc.h>
#include <string.h>
#include "email_parser.h"

email_address_t *init_email_struct() {
    email_address_t *email_struct = (email_address_t *) malloc(sizeof(email_address_t));
    if (email_struct == NULL) {
        return NULL;
    }
    email_struct->username = init_string();
    email_struct->mail_service = init_string();
    email_struct->top_level_domain = init_string();
    return email_struct;
}

int char_is_valid(const char symbol) {
    return (symbol >= '0' && symbol <= '9') ||
           (symbol >= 'a' && symbol <= 'z') ||
           (symbol >= 'A' && symbol <= 'Z') ||
           symbol == '-' || symbol == '.' ||
           symbol == '_' || symbol == '@';
}

email_address_t *parse_email(string_t *email_string) {
    if (email_string == NULL) {
        return NULL;
    }

    for (int i = 0; i < email_string->count; i++) {
        if (!char_is_valid(email_string->string[i])) {
            return NULL;
        }
    }
    char *at_symbol_p = strchr(email_string->string, '@');
    char *last_dot_p = strrchr(email_string->string, '.');
    if (at_symbol_p == NULL || last_dot_p == NULL) {
        return NULL;
    }
    int at_index = (int) (at_symbol_p - email_string->string);
    int last_dot_index = (int) (last_dot_p - email_string->string);

    if (!(at_index > 0 && last_dot_index - at_index > 1 && email_string->count - last_dot_index > 1)) {
        return NULL;
    }
    email_address_t *email_struct = init_email_struct();
    if (email_struct == NULL) {
        return NULL;
    }

    for (int i = 0; i < at_index; i++) {
        add_char(email_struct->username, email_string->string[i]);
    }
    for (int i = at_index + 1; i < last_dot_index; i++) {
        add_char(email_struct->mail_service, email_string->string[i]);
    }
    for (int i = last_dot_index + 1; i < email_string->count; i++) {
        add_char(email_struct->top_level_domain, email_string->string[i]);
    }

    return email_struct;
}

int free_email_struct(email_address_t *email_struct) {
    if (email_struct == NULL) {
        return CODE_ERROR;
    }

    free_string(email_struct->top_level_domain);
    free_string(email_struct->mail_service);
    free_string(email_struct->username);
    free(email_struct);

    return CODE_SUCCESS;
}
