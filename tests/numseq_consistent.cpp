#include "gtest/gtest.h"

extern "C" {
#include "numseq.h"
}

TEST(test_consistent, test1) {
    char arr[] = "1234++++45+1234";
    size_t arr_length = 15;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 0);
    ASSERT_EQ(subseq_max.end, 4);
    ASSERT_EQ(subseq_max.len, 4);
}

TEST(test_consistent, test2) {
    char arr[] = "123451234512345";
    size_t arr_length = 15;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 0);
    ASSERT_EQ(subseq_max.end, 15);
    ASSERT_EQ(subseq_max.len, 15);
}

TEST(test_consistent, test3) {
    char arr[] = "+1234+123++12++";
    size_t arr_length = 15;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 1);
    ASSERT_EQ(subseq_max.end, 5);
    ASSERT_EQ(subseq_max.len, 4);
}

TEST(test_consistent, test4) {
    char arr[] = "1a11111111111aa";
    size_t arr_length = 15;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 2);
    ASSERT_EQ(subseq_max.end, 13);
    ASSERT_EQ(subseq_max.len, 11);
}

TEST(test_consistent, test5) {
    char arr[] = "aaaaaaaaaaaaaaa";
    size_t arr_length = 15;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 0);
    ASSERT_EQ(subseq_max.end, 0);
    ASSERT_EQ(subseq_max.len, 0);
}

TEST(test_consistent, test6) {
    char arr[] = "1a11111111111111111aa";
    size_t arr_length = 21;
    subsequence subseq_max = {0};
    max_number_sequence(arr, &subseq_max, arr_length);
    ASSERT_EQ(subseq_max.start, 2);
    ASSERT_EQ(subseq_max.end, 19);
    ASSERT_EQ(subseq_max.len, 17);
}
