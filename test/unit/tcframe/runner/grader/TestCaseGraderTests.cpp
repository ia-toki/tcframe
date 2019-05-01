#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../util/TestUtils.hpp"
#include "../evaluator/MockEvaluator.hpp"
#include "MockGraderLogger.hpp"
#include "tcframe/runner/grader/TestCaseGrader.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Test;
using ::testing::WhenDynamicCastTo;

namespace tcframe {

class TestCaseGraderTests : public Test {
protected:
    MOCK(Evaluator) evaluator;
    MOCK(GraderLogger) logger;

    TestCase testCase = TestUtils::newTestCase("foo_1");

    GradingOptions options = GradingOptionsBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();
    EvaluationOptions evaluationOptions = EvaluationOptionsBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    TestCaseGrader grader = {&evaluator, &logger};
};

TEST_F(TestCaseGraderTests, Grading_AC) {
    TestCaseVerdict verdict(Verdict::ac());
    map<string, ExecutionResult> executionResults = {{"solution", ExecutionResult()}};
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResult(verdict, executionResults)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate("dir/foo_1.in", "dir/foo_1.out", evaluationOptions));
        EXPECT_CALL(logger, logTestCaseVerdict(verdict));
        EXPECT_CALL(logger, logError(
                WhenDynamicCastTo<FormattedError*>(Pointee(ExecutionResults::asFormattedError(executionResults)))));
    }
    EXPECT_THAT(grader.grade(testCase, options), Eq(verdict));
}

TEST_F(TestCaseGraderTests, Grading_TLE) {
    TestCaseVerdict verdict(Verdict::tle());
    map<string, ExecutionResult> executionResults = {{"solution", ExecutionResult()}};
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResult(verdict, executionResults)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(logger, logTestCaseVerdict(verdict));
        EXPECT_CALL(logger, logError(_)).Times(0);
    }
    EXPECT_THAT(grader.grade(testCase, options), Eq(verdict));
}


}
