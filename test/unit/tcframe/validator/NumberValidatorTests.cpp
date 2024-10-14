#include "gmock/gmock.h"

#include "tcframe/validator/number.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class NumberValidatorTests : public Test {};

TEST_F(NumberValidatorTests, isBetween) {
    EXPECT_FALSE(isBetween(5, 1, 4));
    EXPECT_FALSE(isBetween(5, 6, 10));
    EXPECT_FALSE(isBetween(5, 100, -100));
    EXPECT_TRUE(isBetween(5, 1, 5));
    EXPECT_TRUE(isBetween(5, 5, 10));
    EXPECT_TRUE(isBetween(5, 0, 10));
}

}
