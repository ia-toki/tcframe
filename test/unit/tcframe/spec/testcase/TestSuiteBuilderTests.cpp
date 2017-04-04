#include "gmock/gmock.h"

#include "tcframe/spec/testcase/TestSuite.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestSuiteBuilderTests : public Test {
public:
    static int N;

protected:
    TestSuiteBuilder builder = TestSuiteBuilder().setSlug("foo").setDefinedSubtaskIds({1, 2, 3});
    TestSuiteBuilder builder1 = TestSuiteBuilder().setSlug("foo").setDefinedSubtaskIds({1, 2, 3});
    TestSuiteBuilder builder2 = TestSuiteBuilder().setSlug("foo").setDefinedSubtaskIds({1, 2, 3});
};

int TestSuiteBuilderTests::N;

TEST_F(TestSuiteBuilderTests, Building_Nothing) {
    TestSuite testSuite = builder.build();
    TestSuite expected({
            TestGroup(0, {})});

    EXPECT_THAT(testSuite, Eq(expected));
}

TEST_F(TestSuiteBuilderTests, Building_Lifecycle) {
    TestSuite testSuite = builder
            .setBeforeClosure([&]{N = 2;})
            .setAfterClosure([&]{N *= 4;})
            .addOfficialTestCase([&]{N *= 3;}, "N *= 3")
            .build();
    OfficialTestCaseData* data = (OfficialTestCaseData*) testSuite.testGroups()[1].testCases()[0].data();
    data->closure()();
    EXPECT_THAT(N, Eq(2 * 3 * 4));
}

TEST_F(TestSuiteBuilderTests, Building_OnlySample) {
    TestSuite testSuite1 = builder1
            .newSampleTestCase()
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Input({"30"})
            .build();
    TestSuite testSuite2 = builder2
            .newSampleTestCase()
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Input({"30"})
            .newSampleTestCase() // should be ignored
            .build();
    TestSuite expected({
            TestGroup(0, {
                    TestCaseBuilder()
                            .setName("foo_sample_1")
                            .setSubtaskIds({})
                            .setData(new SampleTestCaseData("10\n20\n", "yes\n"))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_sample_2")
                            .setSubtaskIds({})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()})});

    EXPECT_THAT(testSuite1, Eq(expected));
    EXPECT_THAT(testSuite2, Eq(expected));
}

TEST_F(TestSuiteBuilderTests, Building_OnlyOfficial) {
    TestSuite testSuite = builder
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .build();

    TestSuite expected({
            TestGroup(0, {}),
            TestGroup(-1, {
                    TestCaseBuilder()
                            .setName("foo_1")
                            .setSubtaskIds({-1})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_2")
                            .setSubtaskIds({-1})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})});

    EXPECT_THAT(testSuite, Eq(expected));
}

TEST_F(TestSuiteBuilderTests, Building_Both) {
    TestSuite testSuite1 = builder1
            .newSampleTestCase()
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Input({"30"})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .build();
    TestSuite testSuite2 = builder2
            .newSampleTestCase()
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Input({"30"})
            .newSampleTestCase() // should be ignored
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .build();
    TestSuite expected({
            TestGroup(0, {
                    TestCaseBuilder()
                            .setName("foo_sample_1")
                            .setSubtaskIds({})
                            .setData(new SampleTestCaseData("10\n20\n", "yes\n"))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_sample_2")
                            .setSubtaskIds({})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()}),
            TestGroup(-1, {
                    TestCaseBuilder()
                            .setName("foo_1")
                            .setSubtaskIds({-1})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_2")
                            .setSubtaskIds({-1})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})});

    EXPECT_THAT(testSuite1, Eq(expected));
    EXPECT_THAT(testSuite2, Eq(expected));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups_OnlyOfficial) {
    TestSuite testSuite1 = builder1
            .newTestGroup()
            .Subtasks({1, 2, 3})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup() // must not be ignored
            .Subtasks({2, 3})
            .newTestGroup()
            .Subtasks({3})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .build();
    TestSuite testSuite2 = builder2
            .newTestGroup()
            .Subtasks({1, 2, 3})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup() // must not be ignored
            .Subtasks({2, 3})
            .newTestGroup()
            .Subtasks({3})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .newTestGroup() // should be ignored
            .build();
    TestSuite expected({
            TestGroup(0, {}),
            TestGroup(1, {
                    TestCaseBuilder()
                            .setName("foo_1_1")
                            .setSubtaskIds({1, 2, 3})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_1_2")
                            .setSubtaskIds({1, 2, 3})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()}),
            TestGroup(2, {}),
            TestGroup(3, {
                    TestCaseBuilder()
                            .setName("foo_3_1")
                            .setSubtaskIds({3})
                            .setDescription("N = 3")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_3_2")
                            .setSubtaskIds({3})
                            .setDescription("N = 4")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})});

    EXPECT_THAT(testSuite1, Eq(expected));
    EXPECT_THAT(testSuite2, Eq(expected));
}

TEST_F(TestSuiteBuilderTests, Building_WithGroups_Both) {
    TestSuite testSuite1 = builder1
            .newSampleTestCase()
            .Subtasks({1, 2, 3})
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Subtasks({2, 3})
            .Input({"30"})
            .newTestGroup()
            .Subtasks({1, 2, 3})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup() // must not be ignored
            .Subtasks({2, 3})
            .newTestGroup()
            .Subtasks({3})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .build();
    TestSuite testSuite2 = builder2
            .newSampleTestCase()
            .Subtasks({1, 2, 3})
            .Input({"10", "20"})
            .Output({"yes"})
            .newSampleTestCase()
            .Subtasks({2, 3})
            .Input({"30"})
            .newSampleTestCase() // should be ignored
            .newTestGroup()
            .Subtasks({1, 2, 3})
            .addOfficialTestCase([]{}, "N = 1")
            .addOfficialTestCase([]{}, "N = 2")
            .newTestGroup() // must not be ignored
            .Subtasks({2, 3})
            .newTestGroup()
            .Subtasks({3})
            .addOfficialTestCase([]{}, "N = 3")
            .addOfficialTestCase([]{}, "N = 4")
            .newTestGroup() // should be ignored
            .build();
    TestSuite expected({
            TestGroup(0, {
                    TestCaseBuilder()
                            .setName("foo_sample_1")
                            .setSubtaskIds({1, 2, 3})
                            .setData(new SampleTestCaseData("10\n20\n", "yes\n"))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_sample_2")
                            .setSubtaskIds({2, 3})
                            .setData(new SampleTestCaseData("30\n"))
                            .build()}),
            TestGroup(1, {
                    TestCaseBuilder()
                            .setName("foo_1_1")
                            .setSubtaskIds({1, 2, 3})
                            .setDescription("N = 1")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_1_2")
                            .setSubtaskIds({1, 2, 3})
                            .setDescription("N = 2")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()}),
            TestGroup(2, {}),
            TestGroup(3, {
                    TestCaseBuilder()
                            .setName("foo_3_1")
                            .setSubtaskIds({3})
                            .setDescription("N = 3")
                            .setData(new OfficialTestCaseData([]{}))
                            .build(),
                    TestCaseBuilder()
                            .setName("foo_3_2")
                            .setSubtaskIds({3})
                            .setDescription("N = 4")
                            .setData(new OfficialTestCaseData([]{}))
                            .build()})});

    EXPECT_THAT(testSuite1, Eq(expected));
    EXPECT_THAT(testSuite2, Eq(expected));
}

}
