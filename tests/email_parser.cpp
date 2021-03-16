#include "gtest/gtest.h"
#include <cstring>

extern "C" {
#include "email_parser.h"
#include "../src/email_parser.c"
#include "../src/my_string.c"
}

TEST(init_email_struct, allocation) {
    email_address_t *email = init_email_struct();
    EXPECT_TRUE(
            email != nullptr && email->username != nullptr && email->username != nullptr && email->username != nullptr);
    free_email_struct(email);
}

TEST(char_is_valid, bad_args) {
    ASSERT_FALSE(char_is_valid('\0'));
    ASSERT_FALSE(char_is_valid(' '));
    ASSERT_FALSE(char_is_valid('#'));
    ASSERT_FALSE(char_is_valid('&'));
    ASSERT_FALSE(char_is_valid('?'));
    ASSERT_FALSE(char_is_valid('+'));
}
TEST(char_is_valid, correct_args) {
    ASSERT_TRUE(char_is_valid('@'));
    ASSERT_TRUE(char_is_valid('_'));
    ASSERT_TRUE(char_is_valid('.'));
    ASSERT_TRUE(char_is_valid('-'));
    ASSERT_TRUE(char_is_valid('0'));
    ASSERT_TRUE(char_is_valid('9'));
    ASSERT_TRUE(char_is_valid('a'));
    ASSERT_TRUE(char_is_valid('z'));
    ASSERT_TRUE(char_is_valid('A'));
    ASSERT_TRUE(char_is_valid('Z'));
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

    const int N = 10;
    const char *emails[N] = {"", " a", "a ", "a@.ru", "a@a.", "user!!!", "user$@mail.ru", "@.", "mail.ru",
                             "user @ mail . ru"};
    for (auto & email : emails) {
        string_t *str = create_email_string(email);
        email_address_t *email_struct = parse_email(str);

        EXPECT_EQ(nullptr, email_struct);

        free_string(str);
        free_email_struct(email_struct);
    }
}
TEST(parse_email, correct_email) {
    char email[] = "AaAa_123@gmail.com";
    string_t *str = create_email_string(email);
    email_address_t *email_struct = parse_email(str);

    EXPECT_STREQ("AaAa_123", email_struct->username->string);
    EXPECT_STREQ("gmail", email_struct->mail_service->string);
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
