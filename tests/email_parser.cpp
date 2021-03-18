#include "gtest/gtest.h"
#include <cstring>

extern "C" {
#include "email_parser.h"
}

// func for repetitive code in parse_email() tests
string_t *create_email_string(const char *str_of_chars) {
    int len = strlen(str_of_chars);
    string_t *str = init_string();
    for (int i = 0; i < len; i++) {
        add_char(str, str_of_chars[i]);
    }
    return str;
}
TEST(parse_email, wrong_email) {
    ASSERT_EQ(nullptr, parse_email(nullptr));

    const int N = 12;
    const char *emails[N] = {"", " a", "a ", "a@.ru", "a@a.", "user!!!", "user$@mail.ru", "@.", "mail.ru",
                             "user @ mail . ru", "login+1@mail.ru", "user@mail .ru"};
    for (auto & email : emails) {
        string_t *str = create_email_string(email);
        email_address_t *email_struct = parse_email(str);

        EXPECT_EQ(nullptr, email_struct);

        free_string(str);
        free_email_struct(email_struct);
    }
}
TEST(parse_email, correct_email) {
    char email[] = "A-a.A.abc_123@mail_service.company.com";
    string_t *str = create_email_string(email);
    email_address_t *email_struct = parse_email(str);

    EXPECT_STREQ("A-a.A.abc_123", email_struct->username->string);
    EXPECT_STREQ("mail_service.company", email_struct->mail_service->string);
    EXPECT_STREQ("com", email_struct->top_level_domain->string);

    free_string(str);
    free_email_struct(email_struct);
}

TEST(free_email_struct, null_args) {
    ASSERT_EQ(CODE_ERROR, free_email_struct(nullptr));
}
TEST(free_email_struct, correct_args) {
    auto *email_struct = (email_address_t *) malloc(sizeof(email_address_t));
    email_struct->username = init_string();
    email_struct->mail_service = init_string();
    email_struct->top_level_domain = init_string();
    ASSERT_EQ(CODE_SUCCESS, free_email_struct((email_address_t *) email_struct));
}
