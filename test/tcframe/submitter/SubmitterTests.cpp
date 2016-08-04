#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockTestCaseSubmitter.hpp"
#include "MockSubmitterLogger.hpp"
#include "tcframe/submitter/Submitter.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class SubmitterTests : public Test {
protected:
    static int T;

    Mock(TestCaseSubmitter) testCaseSubmitter;
    Mock(SubmitterLogger) logger;

    TestCase stc1 = TestCaseBuilder().setId("foo_sample_1").setSubtaskIds({1, 2}).build();
    TestCase stc2 = TestCaseBuilder().setId("foo_sample_2").setSubtaskIds({2}).build();
    TestCase tc1 = TestCaseBuilder().setId("foo_1_1").setSubtaskIds({1, 2}).build();
    TestCase tc2 = TestCaseBuilder().setId("foo_2_1").setSubtaskIds({2}).build();
    TestCase tc3 = TestCaseBuilder().setId("foo_3_1").setSubtaskIds({3, 4}).build();
    TestCase tc4 = TestCaseBuilder().setId("foo_4_1").setSubtaskIds({4}).build();
    TestCase tc5 = TestCaseBuilder().setId("foo_4_2").setSubtaskIds({4}).build();

    set<int> subtaskIds = {1, 2, 3, 4};

    TestSuite testSuite = TestSuite({
            TestGroup(0, {stc1, stc2}),
            TestGroup(1, {tc1}),
            TestGroup(2, {tc2}),
            TestGroup(3, {tc3}),
            TestGroup(4, {tc4, tc5})});

    SubmitterConfig config = SubmitterConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();

    SubmitterConfig multipleTestCasesConfig = SubmitterConfigBuilder(config)
            .setHasMultipleTestCasesCount(true)
            .build();

    Submitter submitter = Submitter(&testCaseSubmitter, &logger);

    void SetUp() {
        ON_CALL(testCaseSubmitter, submit(_, _))
                .WillByDefault(Return(Verdict::ac()));
        ON_CALL(testCaseSubmitter, submit(Property(&TestCase::id, StartsWith("foo_2")), _))
                .WillByDefault(Return(Verdict::rte()));
        ON_CALL(testCaseSubmitter, submit(Property(&TestCase::id, StartsWith("foo_3")), _))
                .WillByDefault(Return(Verdict::wa()));
        ON_CALL(testCaseSubmitter, submit(Property(&TestCase::id, StartsWith("foo_4")), _))
                .WillByDefault(Return(Verdict::wa()));
    }
};

int SubmitterTests::T;

TEST_F(SubmitterTests, Submission) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseSubmitter, submit(stc1, config));
        EXPECT_CALL(testCaseSubmitter, submit(stc2, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseSubmitter, submit(tc1, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseSubmitter, submit(tc2, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(3));
        EXPECT_CALL(testCaseSubmitter, submit(tc3, config));
        EXPECT_CALL(logger, logTestGroupIntroduction(4));
        EXPECT_CALL(testCaseSubmitter, submit(tc4, config));
        EXPECT_CALL(testCaseSubmitter, submit(tc5, config));

        EXPECT_CALL(logger, logResult(map<int, Verdict>{
                {1, Verdict::ac()},
                {2, Verdict::rte()},
                {3, Verdict::wa()},
                {4, Verdict::wa()}}));
    }
    submitter.submit(testSuite, subtaskIds, config);
}

TEST_F(SubmitterTests, Submission_MultipleTestCases) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseSubmitter, submit(
                TestCaseBuilder().setId("foo_sample").setSubtaskIds({1, 2}).build(), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(testCaseSubmitter, submit(
                TestCaseBuilder().setId("foo_1").setSubtaskIds({1, 2}).build(), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(testCaseSubmitter, submit(
                TestCaseBuilder().setId("foo_2").setSubtaskIds({2}).build(), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(3));
        EXPECT_CALL(testCaseSubmitter, submit(
                TestCaseBuilder().setId("foo_3").setSubtaskIds({3, 4}).build(), multipleTestCasesConfig));
        EXPECT_CALL(logger, logTestGroupIntroduction(4));
        EXPECT_CALL(testCaseSubmitter, submit(
                TestCaseBuilder().setId("foo_4").setSubtaskIds({4}).build(), multipleTestCasesConfig));

        EXPECT_CALL(logger, logResult(map<int, Verdict>{
                {1, Verdict::ac()},
                {2, Verdict::rte()},
                {3, Verdict::wa()},
                {4, Verdict::wa()}}));
    }
    submitter.submit(testSuite, subtaskIds, multipleTestCasesConfig);
}

}
