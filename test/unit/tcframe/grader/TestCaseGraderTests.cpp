#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../evaluator/MockEvaluator.hpp"
#include "../scorer/MockScorer.hpp"
#include "MockGraderLogger.hpp"
#include "tcframe/grader/TestCaseGrader.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class TestCaseGraderTests : public Test {
protected:
    MOCK(Evaluator) evaluator;
    MOCK(Scorer) scorer;
    MOCK(GraderLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    GraderConfig config = GraderConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();
    EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    EvaluationResult successfulEvaluationResult = EvaluationResult();
    ScoringResult successfulScoringResult = ScoringResultBuilder()
            .setExecutionResult(ExecutionResult())
            .setVerdict(Verdict::ac())
            .build();

    TestCaseGrader grader = TestCaseGrader(&evaluator, &scorer, &logger);

    void SetUp() {
        ON_CALL(evaluator, evaluate(_, _, _))
                .WillByDefault(Return(successfulEvaluationResult));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(Return(successfulScoringResult));
    }
};

TEST_F(TestCaseGraderTests, Grading_AC) {
    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(successfulEvaluationResult)
            .setScoringResult(successfulScoringResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate("dir/foo_1.in", Evaluator::EVALUATION_FILENAME, evaluatorConfig));
        EXPECT_CALL(scorer, score("dir/foo_1.in", "dir/foo_1.out", Evaluator::EVALUATION_FILENAME));
        EXPECT_CALL(logger, logTestCaseGradeSummary(grade));
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }
    EXPECT_THAT(grader.grade(testCase, config), Eq(grade));
}

TEST_F(TestCaseGraderTests, Grading_WA) {
    ScoringResult scoringResult = ScoringResultBuilder()
            .setExecutionResult(ExecutionResult())
            .setVerdict(Verdict::wa())
            .setPrivateInfo("diff")
            .build();
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(scoringResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(successfulEvaluationResult)
            .setScoringResult(scoringResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(scorer, score(_, _, _));
        EXPECT_CALL(logger, logTestCaseGradeSummary(grade));
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }

    EXPECT_THAT(grader.grade(testCase, config), Eq(grade));
}

TEST_F(TestCaseGraderTests, Grading_RTE) {
    EvaluationResult evaluationResult = EvaluationResultBuilder()
            .setVerdict(Verdict::rte())
            .build();
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(evaluationResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(evaluationResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(logger, logTestCaseGradeSummary(grade));
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }
    EXPECT_CALL(scorer, score(_, _, _)).Times(0);

    EXPECT_THAT(grader.grade(testCase, config), Eq(grade));
}

TEST_F(TestCaseGraderTests, Grading_ERR) {
    ScoringResult scoringResult = ScoringResultBuilder()
            .setExecutionResult(ExecutionResultBuilder()
                    .setExitCode(1)
                    .setStandardError("error")
                    .build())
            .setVerdict(Verdict::err())
            .build();
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(scoringResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(successfulEvaluationResult)
            .setScoringResult(scoringResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(scorer, score(_, _, _));
        EXPECT_CALL(logger, logTestCaseGradeSummary(grade));
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }

    EXPECT_THAT(grader.grade(testCase, config), Eq(grade));
}

}
