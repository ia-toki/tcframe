#include "gtest/gtest.h"

#include "tcframe/util.hpp"

using tcframe::Util;

TEST(UtilTest, ToString) {
    EXPECT_EQ("42", Util::toString(42));
}

TEST(UtilTest, TestCaseNameConstructionWithoutTestGroups) {
    EXPECT_EQ("cool_42", Util::constructTestCaseName("cool", -1, 42));
}

TEST(UtilTest, TestCaseNameConstructionWithTestGroups) {
    EXPECT_EQ("cool_7_42", Util::constructTestCaseName("cool", 7, 42));
}
