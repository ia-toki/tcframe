#include "gtest/gtest.h"

#include "tcframe/generator/SampleTestCase.hpp"

#include <set>

using std::set;

using tcframe::SampleTestCase;

TEST(SampleTestCaseTest, Construction) {
    SampleTestCase testCase("7 42\n", {1});

    EXPECT_EQ("7 42\n", testCase.getContent());
    EXPECT_EQ(set<int>{1}, testCase.getSubtaskIds());
}
