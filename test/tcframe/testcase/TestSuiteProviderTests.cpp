#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../io_manipulator/MockIOManipulator.hpp"
#include "tcframe/testcase/TestSuite.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InvokeWithoutArgs;
using ::testing::Test;

namespace tcframe {

int N;

class TestSuiteProviderTests : public Test {
protected:
    Mock(IOManipulator) ioManipulator;

    vector<string> stc1 = {"10"};
    vector<string> stc2 = {"20"};

    OfficialTestCase tc1 = OfficialTestCase([&]{N = 1;}, "N = 1");
    OfficialTestCase tc2 = OfficialTestCase([&]{N = 2;}, "N = 2");
    OfficialTestCase tc3 = OfficialTestCase([&]{N = 3;}, "N = 3");
};

TEST_F(TestSuiteProviderTests, Provide) {
    RawTestSuite rawTestSuite = RawTestSuiteBuilder()
            .addSampleTestCase(stc1)
            .addSampleTestCase(stc2)
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .build();

    EXPECT_CALL(ioManipulator, parseInput(_))
            .Times(2)
            .WillOnce(InvokeWithoutArgs([&]{N = 10;}))
            .WillOnce(InvokeWithoutArgs([&]{N = 20;}));

    TestSuite testSuite = TestSuiteProvider::provide(rawTestSuite, "foo", optional<IOManipulator*>(&ioManipulator));

    EXPECT_THAT(testSuite, Eq(TestSuite({
            TestGroup(0, {
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 0, 1))
                            .setSubtaskIds({-1})
                            .build(),
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 0, 2))
                            .setSubtaskIds({-1})
                            .build()}),
            TestGroup(-1, {
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", -1, 1))
                            .setSubtaskIds({-1})
                            .setDescription("N = 1")
                            .build(),
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", -1, 2))
                            .setSubtaskIds({-1})
                            .setDescription("N = 2")
                            .build()})})));

    EXPECT_THAT((testSuite.testGroups()[0].testCases()[0].applier()(), N), Eq(10));
    EXPECT_THAT((testSuite.testGroups()[0].testCases()[1].applier()(), N), Eq(20));
    EXPECT_THAT((testSuite.testGroups()[1].testCases()[0].applier()(), N), Eq(1));
    EXPECT_THAT((testSuite.testGroups()[1].testCases()[1].applier()(), N), Eq(2));
}

TEST_F(TestSuiteProviderTests, Provide_WithGroups) {
    RawTestSuite rawTestSuite = RawTestSuiteBuilder()
            .addSampleTestCase(stc1, {1, 2})
            .addSampleTestCase(stc2, {1})
            .newTestGroup()
            .Subtasks({1})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .Subtasks({1, 2})
            .addOfficialTestCase(tc3)
            .build();

    EXPECT_CALL(ioManipulator, parseInput(_))
            .Times(2)
            .WillOnce(InvokeWithoutArgs([&]{N = 10;}))
            .WillOnce(InvokeWithoutArgs([&]{N = 20;}));

    TestSuite testSuite = TestSuiteProvider::provide(rawTestSuite, "foo", optional<IOManipulator*>(&ioManipulator));

    EXPECT_THAT(testSuite, Eq(TestSuite({
            TestGroup(0, {
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 0, 1))
                            .setSubtaskIds({1, 2})
                            .build(),
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 0, 2))
                            .setSubtaskIds({1})
                            .build()}),
            TestGroup(1, {
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 1, 1))
                            .setSubtaskIds({1})
                            .setDescription("N = 1")
                            .build(),
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 1, 2))
                            .setSubtaskIds({1})
                            .setDescription("N = 2")
                            .build()}),
            TestGroup(2, {
                    TestCaseBuilder()
                            .setId(TestCaseIdCreator::create("foo", 2, 1))
                            .setSubtaskIds({1, 2})
                            .setDescription("N = 3")
                            .build()})})));

    EXPECT_THAT((testSuite.testGroups()[0].testCases()[0].applier()(), N), Eq(10));
    EXPECT_THAT((testSuite.testGroups()[0].testCases()[1].applier()(), N), Eq(20));
    EXPECT_THAT((testSuite.testGroups()[1].testCases()[0].applier()(), N), Eq(1));
    EXPECT_THAT((testSuite.testGroups()[1].testCases()[1].applier()(), N), Eq(2));
    EXPECT_THAT((testSuite.testGroups()[2].testCases()[0].applier()(), N), Eq(3));
}

TEST_F(TestSuiteProviderTests, Provide_WithInputFinalizer) {
    RawTestSuite rawTestSuite = RawTestSuiteBuilder()
            .setInputFinalizer([] {N *= 2;})
            .addSampleTestCase(stc2)
            .addOfficialTestCase(tc3)
            .build();

    EXPECT_CALL(ioManipulator, parseInput(_))
            .WillOnce(InvokeWithoutArgs([&]{N = 20;}));

    TestSuite testSuite = TestSuiteProvider::provide(rawTestSuite, "foo", optional<IOManipulator*>(&ioManipulator));

    EXPECT_THAT((testSuite.testGroups()[0].testCases()[0].applier()(), N), Eq(20));
    EXPECT_THAT((testSuite.testGroups()[1].testCases()[0].applier()(), N), Eq(3 * 2));
}

}
