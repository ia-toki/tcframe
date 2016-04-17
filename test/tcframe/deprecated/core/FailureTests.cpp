#include "gtest/gtest.h"

#include "tcframe/deprecated/core/Failure.hpp"

using tcframe_old::Failure;

TEST(DeprecatedFailureTest, Construction) {
    Failure failure("some text", 7);

    EXPECT_EQ("some text", failure.getMessage());
    EXPECT_EQ(7, failure.getLevel());
}
