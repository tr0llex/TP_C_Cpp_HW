#include <stdio.h>
#include <malloc.h>
#include "my_string.h"
#include "email_parser.h"

#define CODE_ERROR 1
#define CODE_SUCCESS 0

int main() {
    printf("Please, enter Email address:\n");

    string_t *email_string = init_string();
    if (email_string == NULL) {
        free_string(email_string);
        return CODE_ERROR;
    }

    char last_entered_char = '\0';
    while (scanf("%c", &last_entered_char) == 1 && last_entered_char != '\n') {
        add_char(email_string, last_entered_char);
    }

    email_address_t *email_struct = parse_email(email_string);
    if (email_struct == NULL) {
        printf("Incorrect input\n");
        /*free_string(email_struct->top_level_domain);
        free_string(email_struct->mail_service);
        free_string(email_struct->username);*/
        free(email_struct);
        free_string(email_string);
        return CODE_ERROR;
    }

    printf("\nUsername: %s\nMail-service: %s\nTop-level domain: %s",
           email_struct->username->string, email_struct->mail_service->string, email_struct->top_level_domain->string);

    free_string(email_struct->top_level_domain);
    free_string(email_struct->mail_service);
    free_string(email_struct->username);
    free(email_struct);
    free_string(email_string);
    return CODE_SUCCESS;
}
