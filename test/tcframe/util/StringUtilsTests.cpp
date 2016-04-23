#include "gmock/gmock.h"

#include "tcframe/util/StringUtils.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class StringUtilsTests : public Test {};

TEST_F(StringUtilsTests, SingleStringSplit) {
    EXPECT_THAT(StringUtils::split(" A "), ElementsAre(
            "A"));
}

TEST_F(StringUtilsTests, CommaSeparatedStringsSplit) {
    EXPECT_THAT(StringUtils::split(" A,B, C,   D "), ElementsAre(
            "A", "B", "C", "D"));
}

}
