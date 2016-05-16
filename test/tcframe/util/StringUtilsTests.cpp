#include "gmock/gmock.h"

#include "tcframe/util/StringUtils.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class StringUtilsTests : public Test {};

TEST_F(StringUtilsTests, SplitAndTrimBySpace) {
    EXPECT_THAT(StringUtils::splitAndTrimBySpace(" A B C   D "), ElementsAre(
            "A", "B", "C", "D"));
}

TEST_F(StringUtilsTests, Split_CommaSeparated) {
    EXPECT_THAT(StringUtils::split(" A,B, C,   D ", ','), ElementsAre(
            " A", "B", " C", "   D "));
}

}
