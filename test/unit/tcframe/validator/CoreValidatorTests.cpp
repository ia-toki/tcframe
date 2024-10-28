#include "gmock/gmock.h"

#include "tcframe/validator/core.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class CoreValidatorTests : public Test {};

TEST_F(CoreValidatorTests, valueOf_isBetween) {
    EXPECT_FALSE(valueOf(5).isBetween(1, 4));
    EXPECT_FALSE(valueOf(5).isBetween(6, 10));
    EXPECT_FALSE(valueOf(5).isBetween(100, -100));
    EXPECT_TRUE(valueOf(5).isBetween(1, 5));
    EXPECT_TRUE(valueOf(5).isBetween(5, 10));
    EXPECT_TRUE(valueOf(5).isBetween(0, 10));
}

}
