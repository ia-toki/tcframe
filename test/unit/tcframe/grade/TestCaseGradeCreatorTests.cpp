#include "gmock/gmock.h"

#include "tcframe/grade/TestCaseGradeCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TestCaseGradeCreatorTests : public Test {};

TEST_F(TestCaseGradeCreatorTests, PassesEvaluation) {
    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(EvaluationResult())
            .setScoringResult(ScoringResultBuilder()
                    .setExecutionResult(ExecutionResult())
                    .setVerdict(Verdict::ac())
                    .setPrivateInfo("info")
                    .build())
            .create();

    EXPECT_THAT(grade.verdict(), Eq(Verdict::ac()));
    EXPECT_THAT(grade.evaluationExecutionResult(), Eq(optional<ExecutionResult>(ExecutionResult())));
    EXPECT_THAT(grade.scoringExecutionResult(), Eq(optional<ExecutionResult>(ExecutionResult())));
    EXPECT_THAT(grade.privateScoringInfo(), Eq(optional<string>("info")));
}

TEST_F(TestCaseGradeCreatorTests, FailsEvaluation) {
    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(EvaluationResultBuilder()
                    .setExecutionResult(ExecutionResultBuilder().setExitCode(1).build())
                    .setVerdict(Verdict::rte())
                    .build())
            .create();

    EXPECT_THAT(grade.verdict(), Eq(Verdict::rte()));
    EXPECT_THAT(grade.evaluationExecutionResult(), Eq(optional<ExecutionResult>(
            ExecutionResultBuilder().setExitCode(1).build())));
    EXPECT_FALSE(grade.scoringExecutionResult());
    EXPECT_FALSE(grade.privateScoringInfo());
}

}
