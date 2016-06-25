#include "gmock/gmock.h"

#include <string>

#include "tcframe/testcase/TestSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

using std::string;

namespace tcframe {

class TestSuiteBuilderTests : public Test {
protected:
    vector<string> stc1 = {"10", "20"};
    vector<string> stc2 = {"30"};

    OfficialTestCase tc1 = OfficialTestCase([]{}, "N = 1");
    OfficialTestCase tc2 = OfficialTestCase([]{}, "N = 2");
    OfficialTestCase tc3 = OfficialTestCase([]{}, "N = 3");
    OfficialTestCase tc4 = OfficialTestCase([]{}, "N = 4");

    TestSuiteBuilder builder;
};

TEST_F(TestSuiteBuilderTests, Building) {
    TestSuite testSuite = builder
            .addSampleTestCase(stc1)
            .addSampleTestCase(stc2)
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .build();

    EXPECT_THAT(testSuite.sampleTests(), ElementsAre(
            SampleTestCase("10\n20\n", {-1}), SampleTestCase("30\n", {-1})));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            OfficialTestGroup(-1, {-1}, {tc1, tc2})));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups) {
    TestSuite testSuite = builder
            .newTestGroup()
            .addSampleTestCase(stc1, {1, 2})
            .addSampleTestCase(stc2, {2})
            .setSubtaskIds({1, 2})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .setSubtaskIds({2})
            .addOfficialTestCase(tc3)
            .addOfficialTestCase(tc4)
            .build();

    EXPECT_THAT(testSuite.sampleTests(), ElementsAre(
            SampleTestCase("10\n20\n", {1, 2}), SampleTestCase("30\n", {2})));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            OfficialTestGroup(1, {1, 2}, {tc1, tc2}),
            OfficialTestGroup(2, {2}, {tc3, tc4})));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups_WithoutLastGroup) {
    TestSuite testSuite = builder
            .newTestGroup()
            .addSampleTestCase(stc1, {1, 2})
            .addSampleTestCase(stc2, {2})
            .setSubtaskIds({1, 2})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .setSubtaskIds({2})
            .addOfficialTestCase(tc3)
            .addOfficialTestCase(tc4)
            .newTestGroup()
            .buildWithoutLastTestGroup();

    EXPECT_THAT(testSuite.sampleTests(), ElementsAre(
            SampleTestCase("10\n20\n", {1, 2}), SampleTestCase("30\n", {2})));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            OfficialTestGroup(1, {1, 2}, {tc1, tc2}),
            OfficialTestGroup(2, {2}, {tc3, tc4})));
}

}
