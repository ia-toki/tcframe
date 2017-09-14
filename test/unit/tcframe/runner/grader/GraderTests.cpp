#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../util/TestUtils.hpp"
#include "../aggregator/MockAggregator.hpp"
#include "../client/MockSpecClient.hpp"
#include "MockTestCaseGrader.hpp"
#include "MockGraderLogger.hpp"
#include "tcframe/runner/grader/Grader.hpp"

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

    MOCK(SpecClient) specClient;
    MOCK(TestCaseGrader) testCaseGrader;
    MOCK(Aggregator) aggregator;
    MOCK(GraderLogger) logger;

    TestCase stcA = TestUtils::newSampleTestCase("foo_sample_1");
    TestCase stcB = TestUtils::newSampleTestCase("foo_sample_2");
    TestCase tcA = TestUtils::newTestCase("foo_1");
    TestCase tcB = TestUtils::newTestCase("foo_2");

    TestCase stc1 = TestUtils::newSampleTestCase("foo_sample_1", {1, 2});
    TestCase stc2 = TestUtils::newSampleTestCase("foo_sample_2", {2});
    TestCase tc1 = TestUtils::newTestCase("foo_1_1", {1, 2});
    TestCase tc2 = TestUtils::newTestCase("foo_2_1", {1, 2});
    TestCase tc3 = TestUtils::newTestCase("foo_3_1", {2});

    Verdict stcAVerdict = Verdict(VerdictStatus::ac(), 1);
    Verdict stcBVerdict = Verdict(VerdictStatus::ac(), 2);
    Verdict tcAVerdict = Verdict(VerdictStatus::ac(), 3);
    Verdict tcBVerdict = Verdict(VerdictStatus::ac(), 4);
    Verdict stc1Verdict = Verdict(VerdictStatus::ac(), 5);
    Verdict stc2Verdict = Verdict(VerdictStatus::ac(), 6);
    Verdict tc1Verdict = Verdict(VerdictStatus::ac(), 7);
    Verdict tc2Verdict = Verdict(VerdictStatus::ac(), 8);
    Verdict tc3Verdict = Verdict(VerdictStatus::ac(), 9);

    Verdict mainSubtaskVerdict = Verdict(VerdictStatus::ac(), Subtask::MAIN_POINTS);
    Verdict subtask1Verdict = Verdict(VerdictStatus::wa(), 40);
    Verdict subtask2Verdict = Verdict(VerdictStatus::tle(), 50);
    Verdict verdict = Verdict(VerdictStatus::tle(), 90);

    TestSuite testSuite = TestSuite({
            TestGroup(TestGroup::SAMPLE_ID, {stcA, stcB}),
            TestGroup(TestGroup::MAIN_ID, {tcA, tcB})});

    TestSuite testSuiteWithSubtasks = TestSuite({
            TestGroup(TestGroup::SAMPLE_ID, {stc1, stc2}),
            TestGroup(1, {tc1, tc2}),
            TestGroup(2, {tc3})});

    GradingOptions options = GradingOptionsBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();
    GradingOptions optionsWithSubtasks = GradingOptionsBuilder(options)
            .setSubtaskPoints({40, 50})
            .build();

    Grader grader = Grader(&specClient, &testCaseGrader, &aggregator, &logger);

    void SetUp() {
        ON_CALL(specClient, hasMultipleTestCases())
                .WillByDefault(Return(false));
        ON_CALL(testCaseGrader, grade(stcA, _)).WillByDefault(Return(stcAVerdict));
        ON_CALL(testCaseGrader, grade(stcB, _)).WillByDefault(Return(stcBVerdict));
        ON_CALL(testCaseGrader, grade(tcA, _)).WillByDefault(Return(tcAVerdict));
        ON_CALL(testCaseGrader, grade(tcB, _)).WillByDefault(Return(tcBVerdict));
        ON_CALL(testCaseGrader, grade(stc1, _)).WillByDefault(Return(stc1Verdict));
        ON_CALL(testCaseGrader, grade(stc2, _)).WillByDefault(Return(stc2Verdict));
        ON_CALL(testCaseGrader, grade(tc1, _)).WillByDefault(Return(tc1Verdict));
        ON_CALL(testCaseGrader, grade(tc2, _)).WillByDefault(Return(tc2Verdict));
        ON_CALL(testCaseGrader, grade(tc3, _)).WillByDefault(Return(tc3Verdict));
    }
};

int GraderTests::T;

TEST_F(GraderTests, Grading) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuite));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction("python Sol.py"));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGrader, grade(stcA, options));
        EXPECT_CALL(testCaseGrader, grade(stcB, options));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::MAIN_ID));
        EXPECT_CALL(testCaseGrader, grade(tcA, options));
        EXPECT_CALL(testCaseGrader, grade(tcB, options));

        EXPECT_CALL(aggregator, aggregate(vector<Verdict>{tcAVerdict, tcBVerdict}, Subtask::MAIN_POINTS))
                .WillOnce(Return(mainSubtaskVerdict));

        EXPECT_CALL(logger, logResult(
                map<int, Verdict>{{Subtask::MAIN_ID, mainSubtaskVerdict}},
                mainSubtaskVerdict));
    }
    grader.grade(options);
}

TEST_F(GraderTests, Grading_MultipleTestCases) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuite));
    ON_CALL(specClient, hasMultipleTestCases())
            .WillByDefault(Return(true));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction("python Sol.py"));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGrader, grade(TestUtils::newSampleTestCase("foo_sample"), options))
                .WillOnce(Return(stc1Verdict));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::MAIN_ID));
        EXPECT_CALL(testCaseGrader, grade(TestUtils::newTestCase("foo"), options))
                .WillOnce(Return(tc1Verdict));

        EXPECT_CALL(aggregator, aggregate(vector<Verdict>{tc1Verdict}, Subtask::MAIN_POINTS))
                .WillOnce(Return(mainSubtaskVerdict));

        EXPECT_CALL(logger, logResult(
                map<int, Verdict>{{Subtask::MAIN_ID, mainSubtaskVerdict}},
                mainSubtaskVerdict));
    }
    grader.grade(options);
}

TEST_F(GraderTests, Grading_WithSubtasks) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuiteWithSubtasks));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction("python Sol.py"));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGrader, grade(stc1, options));
        EXPECT_CALL(testCaseGrader, grade(stc2, options));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseGrader, grade(tc1, options));
        EXPECT_CALL(testCaseGrader, grade(tc2, options));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseGrader, grade(tc3, options));

        EXPECT_CALL(aggregator, aggregate(vector<Verdict>{stc1Verdict, tc1Verdict, tc2Verdict}, 40))
                .WillOnce(Return(subtask1Verdict));
        EXPECT_CALL(aggregator, aggregate(
                vector<Verdict>{stc1Verdict, stc2Verdict, tc1Verdict, tc2Verdict, tc3Verdict}, 50))
                .WillOnce(Return(subtask2Verdict));

        EXPECT_CALL(logger, logResult(
                map<int, Verdict>{{1, subtask1Verdict}, {2, subtask2Verdict}},
                verdict));
    }
    grader.grade(optionsWithSubtasks);
}

TEST_F(GraderTests, Grading_WithSubtasks_MultipleTestCases) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuiteWithSubtasks));
    ON_CALL(specClient, hasMultipleTestCases())
            .WillByDefault(Return(true));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction("python Sol.py"));
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGrader, grade(TestUtils::newSampleTestCase("foo_sample", {1, 2}), options))
                .WillOnce(Return(stc1Verdict));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseGrader, grade(TestUtils::newTestCase("foo_1", {1, 2}), options))
                .WillOnce(Return(tc1Verdict));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseGrader, grade(TestUtils::newTestCase("foo_2", {2}), options))
                .WillOnce(Return(tc2Verdict));

        EXPECT_CALL(aggregator, aggregate(vector<Verdict>{stc1Verdict, tc1Verdict}, 40))
                .WillOnce(Return(subtask1Verdict));
        EXPECT_CALL(aggregator, aggregate(vector<Verdict>{stc1Verdict, tc1Verdict, tc2Verdict}, 50))
                .WillOnce(Return(subtask2Verdict));

        EXPECT_CALL(logger, logResult(
                map<int, Verdict>{{1, subtask1Verdict}, {2, subtask2Verdict}},
                verdict));
    }
    grader.grade(optionsWithSubtasks);
}

}
