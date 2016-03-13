#include "gtest/gtest.h"

#include "tcframe/generator/OfficialTestCase.hpp"

#include <set>

using std::set;

using tcframe::OfficialTestCase;

TEST(OfficialTestCaseTest, Construction) {
    int X;
    OfficialTestCase testCase([&] { X = 42; }, "X = 42", {1});

    EXPECT_EQ("X = 42", testCase.getDescription());

    X = 7;
    EXPECT_EQ(7, X);

    testCase.apply();
    EXPECT_EQ(42, X);

    EXPECT_EQ(set<int>{1}, testCase.getSubtaskIds());
}