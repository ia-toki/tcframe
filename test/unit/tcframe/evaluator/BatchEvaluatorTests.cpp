#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "../verdict/MockVerdictCreator.hpp"
#include "scorer/MockScorer.hpp"
#include "tcframe/evaluator/BatchEvaluator.hpp"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class BatchEvaluatorTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    MOCK(VerdictCreator) verdictCreator;
    MOCK(Scorer) scorer;

    EvaluatorConfig config = EvaluatorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    BatchEvaluator evaluator = BatchEvaluator(&os, &verdictCreator, &scorer);

    void SetUp() {
        ON_CALL(os, execute(_))
                .WillByDefault(Return(ExecutionResult()));
        ON_CALL(verdictCreator, fromExecutionResult(_))
                .WillByDefault(Return(optional<Verdict>()));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(Return(ScoringResult(Verdict(), ExecutionResult())));
    }
};

TEST_F(BatchEvaluatorTests, Evaluation_AC) {
    ExecutionRequest executionRequest = ExecutionRequestBuilder()
            .setCommand("python Sol.py")
            .setInputFilename("dir/foo_1.in")
            .setOutputFilename(Evaluator::EVALUATION_OUT_FILENAME)
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(executionRequest));
        EXPECT_CALL(scorer, score("dir/foo_1.in", "dir/foo_1.out", Evaluator::EVALUATION_OUT_FILENAME));
    }

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResult(
            Verdict(VerdictStatus::ac()),
            {{"solution", ExecutionResult()}, {"scorer", ExecutionResult()}})));
}

TEST_F(BatchEvaluatorTests, Evaluation_FromScorer) {
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(ScoringResult(Verdict(VerdictStatus::wa()), ExecutionResult())));
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(_));
        EXPECT_CALL(scorer, score(_, _, _));
    }

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResult(
            Verdict(VerdictStatus::wa()),
            {{"solution", ExecutionResult()}, {"scorer", ExecutionResult()}})));
}

TEST_F(BatchEvaluatorTests, Evaluation_FromSolution) {
    ON_CALL(verdictCreator, fromExecutionResult(_))
            .WillByDefault(Return(optional<Verdict>(Verdict(VerdictStatus::rte()))));
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(_));
        EXPECT_CALL(scorer, score(_, _, _)).Times(0);
    }

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResult(
            Verdict(VerdictStatus::rte()),
            {{"solution", ExecutionResult()}})));
}

}
