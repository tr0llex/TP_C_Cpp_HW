#include "gtest/gtest.h"

extern "C" {
#include "testing.h"
}

TEST(return_five, null_arguments) {
    EXPECT_EQ(5, testing_return_five());
}