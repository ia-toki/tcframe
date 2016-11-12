#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../util/TestUtils.hpp"
#include "MockTestCaseGrader.hpp"
#include "MockGraderLogger.hpp"
#include "tcframe/grader/Grader.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class GraderTests : public Test {
protected:
    static int T;

    MOCK(TestCaseGrader) testCaseGrader;
    MOCK(GraderLogger) logger;

    TestCase stc1 = TestUtils::createFakeTestCase("foo_sample_1", {1, 2});
    TestCase stc2 = TestUtils::createFakeTestCase("foo_sample_2", {2});
    TestCase tc1 = TestUtils::createFakeTestCase("foo_1_1", {1, 2});
    TestCase tc2 = TestUtils::createFakeTestCase("foo_2_1", {2});
    TestCase tc3 = TestUtils::createFakeTestCase("foo_3_1", {3, 4});
    TestCase tc4 = TestUtils::createFakeTestCase("foo_4_1", {4});
    TestCase tc5 = TestUtils::createFakeTestCase("foo_4_2", {4});

    set<int> subtaskIds = {1, 2, 3, 4};

    TestSuite testSuite = TestSuite({
            TestGroup(0, {stc1, stc2}),
            TestGroup(1, {tc1}),
            TestGroup(2, {tc2}),
            TestGroup(3, {tc3}),
            TestGroup(4, {tc4, tc5})});

    GraderConfig config = GraderConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();

    GraderConfig multipleTestCasesConfig = GraderConfigBuilder(config)
            .setHasMultipleTestCases(true)
            .build();

    Grader grader = Grader(&testCaseGrader, &logger);

    void SetUp() {
        ON_CALL(testCaseGrader, grade(_, _))
                .WillByDefault(Return(Verdict::ac()));
        ON_CALL(testCaseGrader, grade(Property(&TestCase::id, StartsWith("foo_2")), _))
                .WillByDefault(Return(Verdict::rte()));
        ON_CALL(testCaseGrader, grade(Property(&TestCase::id, StartsWith("foo_3")), _))
                .WillByDefault(Return(Verdict::wa()));
        ON_CALL(testCaseGrader, grade(Property(&TestCase::id, StartsWith("foo_4")), _))
                .WillByDefault(Return(Verdict::tle()));
    }
};

int GraderTests::T;

TEST_F(GraderTests, Grading) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseGrader, grade(stc1, config));
        EXPECT_CALL(testCaseGrader, grade(stc2, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseGrader, grade(tc1, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseGrader, grade(tc2, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(3));
        EXPECT_CALL(testCaseGrader, grade(tc3, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(4));
        EXPECT_CALL(testCaseGrader, grade(tc4, config));
        EXPECT_CALL(testCaseGrader, grade(tc5, config));

        EXPECT_CALL(logger, logResult(map<int, Verdict>{
                {1, Verdict::ac()},
                {2, Verdict::rte()},
                {3, Verdict::wa()},
                {4, Verdict::tle()}}));
    }
    grader.grade(testSuite, subtaskIds, config);
}

TEST_F(GraderTests, Grading_MultipleTestCases) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseGrader, grade(
                TestUtils::createFakeTestCase("foo_sample", {1, 2}), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseGrader, grade(
                TestUtils::createFakeTestCase("foo_1", {1, 2}), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseGrader, grade(
                TestUtils::createFakeTestCase("foo_2", {2}), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(3));
        EXPECT_CALL(testCaseGrader, grade(
                TestUtils::createFakeTestCase("foo_3", {3, 4}), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(4));
        EXPECT_CALL(testCaseGrader, grade(
                TestUtils::createFakeTestCase("foo_4", {4}), multipleTestCasesConfig));

        EXPECT_CALL(logger, logResult(map<int, Verdict>{
                {1, Verdict::ac()},
                {2, Verdict::rte()},
                {3, Verdict::wa()},
                {4, Verdict::tle()}}));
    }
    grader.grade(testSuite, subtaskIds, multipleTestCasesConfig);
}

}
