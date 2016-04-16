#include "gmock/gmock.h"

#include "tcframe/testcase/TestCase.hpp"
#include "tcframe/testcase/TestSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

using tcframe::TestCase;
using tcframe::TestGroup;
using tcframe::TestSuite;
using tcframe::TestSuiteBuilder;

namespace tcframe {

class TestSuiteBuilderTests : public Test {
protected:
    const TestCase tc1 = TestCase([]{}, "N = 1");
    const TestCase tc2 = TestCase([]{}, "N = 2");
    const TestCase tc3 = TestCase([]{}, "N = 3");
    const TestCase tc4 = TestCase([]{}, "N = 4");

    TestSuiteBuilder builder;
};

TEST_F(TestSuiteBuilderTests, CanBuildWithoutTestGroups) {
    TestSuite testSuite = builder
            .addTestCase(tc1)
            .addTestCase(tc2)
            .build();

    EXPECT_THAT(testSuite.testCases(), ElementsAre(
            TestGroup(-1, {-1}, {tc1, tc2})));
}

TEST_F(TestSuiteBuilderTests, CanBuildWithTestGroups) {
    TestSuite testSuite = builder
            .newTestGroup()
            .setConstraintGroupIds({1, 2})
            .addTestCase(tc1)
            .addTestCase(tc2)
            .newTestGroup()
            .setConstraintGroupIds({2})
            .addTestCase(tc3)
            .addTestCase(tc4)
            .build();

    EXPECT_THAT(testSuite.testCases(), ElementsAre(
            TestGroup(1, {1, 2}, {tc1, tc2}),
            TestGroup(2, {2}, {tc3, tc4})));
}

}
