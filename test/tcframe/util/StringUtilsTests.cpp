#include "gmock/gmock.h"

#include "tcframe/util/StringUtils.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class StringUtilsTests : public Test {};

TEST_F(StringUtilsTests, ToString) {
    EXPECT_THAT(StringUtils::toString(42), Eq("42"));
}

TEST_F(StringUtilsTests, ToNumber) {
    EXPECT_THAT(StringUtils::toNumber<int>("42"), Eq(optional<int>(42)));
    EXPECT_THAT(StringUtils::toNumber<int>("-42"), Eq(optional<int>(-42)));
    EXPECT_THAT(StringUtils::toNumber<int>("abc"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>("42 123"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>("42abc"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>(" 42 "), Eq(optional<int>()));
}

TEST_F(StringUtilsTests, Trim) {
    EXPECT_THAT(StringUtils::trim("   \t "), Eq(""));
    EXPECT_THAT(StringUtils::trim("clean"), Eq("clean"));
    EXPECT_THAT(StringUtils::trim("  \t foo  bar   \r\n"), Eq("foo  bar"));
}

TEST_F(StringUtilsTests, SplitAndTrimBySpace) {
    EXPECT_THAT(StringUtils::splitAndTrimBySpace(" A B C   D "), ElementsAre(
            "A", "B", "C", "D"));
}

TEST_F(StringUtilsTests, Split) {
    EXPECT_THAT(StringUtils::split(" A,B, C,   D ", ','), ElementsAre(
            " A", "B", " C", "   D "));
}

}
