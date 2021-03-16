#ifndef INCLUDE_EMAIL_PARSER_H_
#define INCLUDE_EMAIL_PARSER_H_

#include <my_string.h>

typedef struct email_address_t {
    string_t *username;
    string_t *mail_service;
    string_t *top_level_domain;
} email_address_t;

email_address_t *init_email_struct();
int char_is_valid(char);
email_address_t *parse_email(string_t *);
int free_email_struct(email_address_t *);

#endif  // INCLUDE_EMAIL_PARSER_H_
