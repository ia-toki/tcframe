#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "../verdict/MockTestCaseVerdictParser.hpp"
#include "scorer/MockScorer.hpp"
#include "tcframe/runner/evaluator/BatchEvaluator.hpp"

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
    MOCK(TestCaseVerdictParser) testCaseVerdictParser;
    MOCK(Scorer) scorer;

    EvaluationOptions options = EvaluationOptionsBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    BatchEvaluator evaluator = {&os, &testCaseVerdictParser, &scorer};

    void SetUp() {
        ON_CALL(os, execute(_))
                .WillByDefault(Return(ExecutionResult()));
        ON_CALL(testCaseVerdictParser, parseExecutionResult(_))
                .WillByDefault(Return(optional<TestCaseVerdict>()));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(Return(ScoringResult(TestCaseVerdict(), ExecutionResult())));
    }
};

TEST_F(BatchEvaluatorTests, Evaluation_AC) {
    auto executionRequest = ExecutionRequestBuilder()
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

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::ac()),
            {{"solution", ExecutionResult()}, {"scorer", ExecutionResult()}})));
}

TEST_F(BatchEvaluatorTests, Evaluation_FromScorer) {
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(ScoringResult(TestCaseVerdict(Verdict::wa()), ExecutionResult())));
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(_));
        EXPECT_CALL(scorer, score(_, _, _));
    }

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::wa()),
            {{"solution", ExecutionResult()}, {"scorer", ExecutionResult()}})));
}

TEST_F(BatchEvaluatorTests, Evaluation_FromSolution) {
    ON_CALL(testCaseVerdictParser, parseExecutionResult(_))
            .WillByDefault(Return(optional<TestCaseVerdict>(TestCaseVerdict(Verdict::rte()))));
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(_));
        EXPECT_CALL(scorer, score(_, _, _)).Times(0);
    }

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::rte()),
            {{"solution", ExecutionResult()}})));
}

}
