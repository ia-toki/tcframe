#include "gtest/gtest.h"

#include "tcframe/core/Failure.hpp"

using tcframe::Failure;

TEST(FailureTest, Construction) {
    Failure failure("some text", 7);

    EXPECT_EQ("some text", failure.getMessage());
    EXPECT_EQ(7, failure.getLevel());
}
