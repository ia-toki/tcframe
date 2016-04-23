#include "gmock/gmock.h"

#include "tcframe/testcase/TestSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class TestSuiteBuilderTests : public Test {
protected:
    OfficialTestCase tc1 = OfficialTestCase([]{}, "N = 1");
    OfficialTestCase tc2 = OfficialTestCase([]{}, "N = 2");
    OfficialTestCase tc3 = OfficialTestCase([]{}, "N = 3");
    OfficialTestCase tc4 = OfficialTestCase([]{}, "N = 4");

    TestSuiteBuilder builder;
};

TEST_F(TestSuiteBuilderTests, CanBuildWithoutTestGroups) {
    TestSuite testSuite = builder
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .build();

    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            TestGroup(-1, {-1}, {tc1, tc2})));
}

TEST_F(TestSuiteBuilderTests, CanBuildWithTestGroups) {
    TestSuite testSuite = builder
            .newTestGroup()
            .setConstraintGroupIds({1, 2})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .setConstraintGroupIds({2})
            .addOfficialTestCase(tc3)
            .addOfficialTestCase(tc4)
            .build();

    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            TestGroup(1, {1, 2}, {tc1, tc2}),
            TestGroup(2, {2}, {tc3, tc4})));
}

}
