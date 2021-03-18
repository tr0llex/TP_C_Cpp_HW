#include <stdio.h>
#include "my_string.h"
#include "email_parser.h"

int main() {
    printf("Please, enter Email address:\n");

    string_t *email_string = init_string();
    if (email_string == NULL) {
        return CODE_ERROR;
    }

    char last_entered_char = '\0';
    while (scanf("%c", &last_entered_char) == 1 && last_entered_char != '\n') {
        add_char(email_string, last_entered_char);
    }

    email_address_t *email_struct = parse_email(email_string);
    if (email_struct == NULL) {
        free_string(email_string);
        printf("Incorrect input\n");
        return CODE_ERROR;
    }

    printf("\nUsername: %s\nMail-service: %s\nTop-level domain: %s",
           email_struct->username->string, email_struct->mail_service->string, email_struct->top_level_domain->string);

    free_email_struct(email_struct);
    free_string(email_string);
    return CODE_SUCCESS;
}
