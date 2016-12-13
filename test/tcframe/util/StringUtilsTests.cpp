#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/util/StringUtils.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class StringUtilsTests : public Test {};

TEST_F(StringUtilsTests, ToString) {
    EXPECT_THAT(StringUtils::toString(42), Eq("42"));
}

TEST_F(StringUtilsTests, StreamToString) {
    EXPECT_THAT(StringUtils::streamToString(new istringstream(" hello, world! ")), Eq(" hello, world! "));
}

TEST_F(StringUtilsTests, ToNumber) {
    EXPECT_THAT(StringUtils::toNumber<int>("42"), Eq(optional<int>(42)));
    EXPECT_THAT(StringUtils::toNumber<int>("-42"), Eq(optional<int>(-42)));
    EXPECT_THAT(StringUtils::toNumber<int>("abc"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>("42 123"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>("42abc"), Eq(optional<int>()));
    EXPECT_THAT(StringUtils::toNumber<int>(" 42 "), Eq(optional<int>()));
}

TEST_F(StringUtilsTests, Interpolate) {
    EXPECT_THAT(StringUtils::interpolate("abc %d def", 42), Eq("abc 42 def"));
}

TEST_F(StringUtilsTests, Replace) {
    EXPECT_THAT(StringUtils::replace("hello$world", '$', "\\$"), Eq("hello\\$world"));
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
