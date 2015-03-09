#include "gtest/gtest.h"

#include "tcframe/util.hpp"

using tcframe::Util;

TEST(UtilTest, ToString) {
    EXPECT_EQ("42", Util::toString(42));
}

TEST(UtilTest, Split) {
    EXPECT_EQ((vector<string>{"A", "B", "C"}), Util::split("A, B, C"));
}

TEST(UtilTest, SampleTestCaseNameConstruction) {
    EXPECT_EQ("cool_sample_42", Util::constructTestCaseName("cool", 0, 42));
}

TEST(UtilTest, OfficialTestCaseNameConstructionWithoutTestGroups) {
    EXPECT_EQ("cool_42", Util::constructTestCaseName("cool", -1, 42));
}

TEST(UtilTest, OfficialTestCaseNameConstructionWithTestGroups) {
    EXPECT_EQ("cool_7_42", Util::constructTestCaseName("cool", 7, 42));
}
