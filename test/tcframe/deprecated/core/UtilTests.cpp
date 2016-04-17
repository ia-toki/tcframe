#include "gtest/gtest.h"

#include "tcframe/deprecated/core/Util.hpp"

#include <vector>
#include <string>

using std::vector;
using std::string;

using tcframe_old::Util;

TEST(DeprecatedUtilTest, ToString) {
    EXPECT_EQ("42", Util::toString(42));
}

TEST(DeprecatedUtilTest, Split) {
    EXPECT_EQ((vector<string>{"A", "B", "C"}), Util::split("A, B, C"));
}

TEST(DeprecatedUtilTest, SampleTestCaseNameConstruction) {
    EXPECT_EQ("cool_sample_42", Util::constructTestCaseName("cool", 0, 42));
}

TEST(DeprecatedUtilTest, OfficialTestCaseNameConstructionWithoutTestGroups) {
    EXPECT_EQ("cool_42", Util::constructTestCaseName("cool", -1, 42));
}

TEST(DeprecatedUtilTest, OfficialTestCaseNameConstructionWithTestGroups) {
    EXPECT_EQ("cool_7_42", Util::constructTestCaseName("cool", 7, 42));
}
