#include "gtest/gtest.h"

extern "C" {
#include "my_string.h"
#include "../src/my_string.c"
}

//int free_string(string_t *str)
TEST(free_string, null_args) {
    ASSERT_EQ(CODE_ERROR, free_string(nullptr));
}
TEST(free_string, correct_args) {
    auto *str = (string_t *) malloc(sizeof(string_t));
    str->string = (char*) malloc(10 * sizeof(char));
    ASSERT_EQ(CODE_SUCCESS, free_string(str));
}

// done
TEST(init_string, allocation) {
    string_t *str = init_string();
    EXPECT_TRUE(str != nullptr && str->string != nullptr);
    EXPECT_TRUE(str->capacity == 2 && str->count == 0);
    free_string(str);
}

//done
TEST(enlarge_string, null_args) {
    ASSERT_EQ(CODE_ERROR, enlarge_string(nullptr));
}
TEST(enlarge_string, correct_args) {
    string_t *str = init_string();
    EXPECT_EQ(CODE_SUCCESS, enlarge_string(str));
    free_string(str);
}

// done
TEST(add_char, null_args) {
    ASSERT_EQ(CODE_ERROR, add_char(nullptr, 'V'));
}
TEST(add_char, correct_args) {
    string_t *str = init_string();
    EXPECT_EQ(CODE_SUCCESS, add_char(str, 'V'));
    EXPECT_STREQ("V", str->string);
    free_string(str);
}
TEST(add_char, sequence_of_additions) {
    string_t *str = init_string();
    add_char(str, 'a');
    add_char(str, '!');
    add_char(str, 'G');
    EXPECT_STREQ("a!G", str->string);
    free_string(str);
}
