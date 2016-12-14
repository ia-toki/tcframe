#include "gmock/gmock.h"
#include "../helper.hpp"
#include "../mock.hpp"

#include <sstream>

#include "../evaluator/MockEvaluator.hpp"
#include "../scorer/MockScorer.hpp"
#include "MockGraderLogger.hpp"
#include "tcframe/grader/Grader.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Return;
using ::testing::Test;
using ::testing::WithArg;

using std::istringstream;

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

    TestCaseGrader grader = TestCaseGrader(&evaluator, &scorer, &logger);

    Captor<string> evaluationCaptor1;
    Captor<string> evaluationCaptor2;

    void SetUp() {
        ON_CALL(evaluator, evaluate(_, _, _))
                .WillByDefault(DoAll(
                        WithArg<1>(Invoke(&evaluationCaptor1, &Captor<string>::capture)),
                        InvokeWithoutArgs([] {return EvaluationResultBuilder().build();})));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(DoAll(
                        WithArg<2>(Invoke(&evaluationCaptor2, &Captor<string>::capture)),
                        InvokeWithoutArgs([] {return ScoringResultBuilder()
                                .setExecutionResult(ExecutionResultBuilder()
                                        .setInfo(ExecutionInfoBuilder().setExitCode(0).build())
                                        .build())
                                .setVerdict(Verdict::ac())
                                .build();})));
    }
};

TEST_F(TestCaseGraderTests, Grading_AC) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate("dir/foo_1.in", _, evaluatorConfig));
        EXPECT_CALL(scorer, score("dir/foo_1.in", "dir/foo_1.out", _));
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::ac()));
    }
    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::ac()));

    EXPECT_THAT(evaluationCaptor1.arg(), Eq(evaluationCaptor2.arg()));
}

TEST_F(TestCaseGraderTests, Grading_WA) {
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(ScoringResultBuilder()
                    .setExecutionResult(ExecutionResultBuilder()
                            .setInfo(ExecutionInfoBuilder().setExitCode(0).build())
                            .build())
                    .setVerdict(Verdict::wa())
                    .setMessage("diff")
                    .build()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(scorer, score(_, _, _));
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::wa()));
        EXPECT_CALL(logger, logTestCaseScoringMessage("diff"));
    }

    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::wa()));
}

TEST_F(TestCaseGraderTests, Grading_RTE) {
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResultBuilder().setVerdict(Verdict::rte()).build()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(_, _, _));
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::rte()));
        EXPECT_CALL(logger, logSolutionExecutionFailure(_));
    }
    EXPECT_CALL(scorer, score(_, _, _)).Times(0);

    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::rte()));
}

}
