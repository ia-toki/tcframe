#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class TestSuiteBuilderTests : public Test {
protected:
    TestSuiteBuilder builder = TestSuiteBuilder().setSlug("foo");
};

TEST_F(TestSuiteBuilderTests, Building) {
    TestSuite testSuite = builder
            .addSampleTestCase({"10", "20"})
            .addSampleTestCase({"30"})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .build();

    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            TestGroup(0, {
                    TestCaseBuilder()
                            .setId("foo_sample_1")
                            .setSubtaskIds({-1})
                            .setData(new SampleTestCaseData("10\n20\n"))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_sample_2")
                            .setSubtaskIds({-1})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()}),
            TestGroup(-1, {
                    TestCaseBuilder()
                            .setId("foo_1")
                            .setSubtaskIds({-1})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_2")
                            .setSubtaskIds({-1})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups) {
    TestSuite testSuite = builder
            .addSampleTestCase({"10", "20"}, {1, 2})
            .addSampleTestCase({"30"}, {2})
            .newTestGroup()
            .Subtasks({1, 2})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup()
            .Subtasks({2})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .build();

    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            TestGroup(0, {
                    TestCaseBuilder()
                            .setId("foo_sample_1")
                            .setSubtaskIds({1, 2})
                            .setData(new SampleTestCaseData("10\n20\n"))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_sample_2")
                            .setSubtaskIds({2})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()}),
            TestGroup(1, {
                    TestCaseBuilder()
                            .setId("foo_1_1")
                            .setSubtaskIds({1, 2})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_1_2")
                            .setSubtaskIds({1, 2})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()}),
            TestGroup(2, {
                    TestCaseBuilder()
                            .setId("foo_2_1")
                            .setSubtaskIds({2})
                            .setDescription("N = 3")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_2_2")
                            .setSubtaskIds({2})
                            .setDescription("N = 4")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups_WithoutLastGroup) {
    TestSuite testSuite = builder
            .addSampleTestCase({"10", "20"}, {1, 2})
            .addSampleTestCase({"30"}, {2})
            .newTestGroup()
            .Subtasks({1, 2})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup()
            .Subtasks({2})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .newTestGroup()
            .buildWithoutLastTestGroup();

    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            TestGroup(0, {
                    TestCaseBuilder()
                            .setId("foo_sample_1")
                            .setSubtaskIds({1, 2})
                            .setData(new SampleTestCaseData("10\n20\n"))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_sample_2")
                            .setSubtaskIds({2})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()}),
            TestGroup(1, {
                    TestCaseBuilder()
                            .setId("foo_1_1")
                            .setSubtaskIds({1, 2})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_1_2")
                            .setSubtaskIds({1, 2})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()}),
            TestGroup(2, {
                    TestCaseBuilder()
                            .setId("foo_2_1")
                            .setSubtaskIds({2})
                            .setDescription("N = 3")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setId("foo_2_2")
                            .setSubtaskIds({2})
                            .setDescription("N = 4")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})));
}

}
