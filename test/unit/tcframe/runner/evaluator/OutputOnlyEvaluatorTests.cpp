#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <ios>
#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "../verdict/MockTestCaseVerdictParser.hpp"
#include "scorer/MockScorer.hpp"
#include "tcframe/runner/evaluator/OutputOnlyEvaluator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class OutputOnlyEvaluatorTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    MOCK(TestCaseVerdictParser) testCaseVerdictParser;
    MOCK(Scorer) scorer;

    EvaluationOptions options = EvaluationOptionsBuilder()
            .setSolutionCommand("submissions")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    OutputOnlyEvaluator evaluator = {&os, &testCaseVerdictParser, &scorer};

    void SetUp() {
        ON_CALL(os, openForReading(_))
                .WillByDefault(Return(new istringstream("1 2\n")));
        ON_CALL(os, execute(_))
                .WillByDefault(Return(ExecutionResult()));
        ON_CALL(testCaseVerdictParser, parseExecutionResult(_))
                .WillByDefault(Return(optional<TestCaseVerdict>()));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(Return(ScoringResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));
    }
};

TEST_F(OutputOnlyEvaluatorTests, Evaluation_AC) {
    EXPECT_CALL(os, execute(_)).Times(0);
    EXPECT_CALL(scorer, score("tc/foo_1.in", "tc/foo_1.out", "submissions/foo_1.out"));

    EXPECT_THAT(evaluator.evaluate("tc/foo_1.in", "tc/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::ac()),
            {{"scorer", ExecutionResult()}})));
}

TEST_F(OutputOnlyEvaluatorTests, Evaluation_FromScorer) {
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(ScoringResult(TestCaseVerdict(Verdict::wa()), ExecutionResult())));

    EXPECT_THAT(evaluator.evaluate("tc/foo_1.in", "tc/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::wa()),
            {{"scorer", ExecutionResult()}})));
}

TEST_F(OutputOnlyEvaluatorTests, Evaluation_MissingSubmission_WA) {
    auto missing = new istringstream("");
    missing->setstate(std::ios::badbit);
    EXPECT_CALL(os, openForReading("submissions/foo_1.out")).WillOnce(Return(missing));
    EXPECT_CALL(scorer, score(_, _, _)).Times(0);

    auto expectedError = ExecutionResultBuilder()
            .setStandardError("Submitted output not found: submissions/foo_1.out\n")
            .build();
    EXPECT_THAT(evaluator.evaluate("tc/foo_1.in", "tc/foo_1.out", options), Eq(EvaluationResult(
            TestCaseVerdict(Verdict::wa()),
            {{"scorer", expectedError}})));
}

TEST_F(OutputOnlyEvaluatorTests, Evaluation_SubmissionNameFromOutputPathOnly) {
    EXPECT_CALL(os, openForReading("submissions/bar_3.out"));
    EXPECT_CALL(scorer, score("dir/bar_3.in", "dir/bar_3.out", "submissions/bar_3.out"));

    evaluator.evaluate("dir/bar_3.in", "dir/bar_3.out", options);
}

TEST_F(OutputOnlyEvaluatorTests, Generation_RunsSolution) {
    auto executionRequest = ExecutionRequestBuilder()
            .setCommand("python Sol.py")
            .setInputFilename("tc/foo_1.in")
            .setOutputFilename("tc/foo_1.out")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();
    auto solutionOptions = EvaluationOptionsBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();
    EXPECT_CALL(os, execute(executionRequest));

    evaluator.generate("tc/foo_1.in", "tc/foo_1.out", solutionOptions);
}

}
