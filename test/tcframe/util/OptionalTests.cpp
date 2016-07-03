#include "gmock/gmock.h"

#include "tcframe/util/optional.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class OptionalTests : public Test {};

TEST_F(OptionalTests, Empty) {
    optional<int> x;
    EXPECT_FALSE(x);
}

TEST_F(OptionalTests, Present) {
    optional<int> x(7);
    EXPECT_TRUE(x);
    EXPECT_THAT(x.value(), Eq(7));
}

TEST_F(OptionalTests, ValueOr) {
    EXPECT_THAT(optional<int>(7).value_or(10), Eq(7));
    EXPECT_THAT(optional<int>().value_or(10), Eq(10));
}

}
