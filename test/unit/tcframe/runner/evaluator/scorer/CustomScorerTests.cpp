#include "gmock/gmock.h"
#include "../../../mock.hpp"

#include <sstream>

#include "../../os/MockOperatingSystem.hpp"
#include "../../verdict/MockTestCaseVerdictParser.hpp"
#include "tcframe/runner/evaluator/scorer/CustomScorer.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

using std::istringstream;

namespace tcframe {

class CustomScorerTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    MOCK(TestCaseVerdictParser) testCaseVerdictParser;
    string scorerCommand = "./scorer";

    CustomScorer scorer = {&os, &testCaseVerdictParser, scorerCommand};

    void SetUp() {
        ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResult()));
        ON_CALL(os, openForReading(_)).WillByDefault(Return(new istringstream()));
        ON_CALL(testCaseVerdictParser, parseStream(_)).WillByDefault(Return(TestCaseVerdict(Verdict::ac())));
    }
};

TEST_F(CustomScorerTests, Scoring_Successful) {
    EXPECT_THAT(scorer.score("1.in", "1.out", "eval.out"), Eq(
            ScoringResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));
}

TEST_F(CustomScorerTests, Scoring_Crashed) {
    auto executionResult = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("crashed")
            .build();
    ON_CALL(os, execute(_)).WillByDefault(Return(executionResult));

    EXPECT_THAT(scorer.score("1.in", "1.out", "eval.out"), Eq(
            ScoringResult(TestCaseVerdict(Verdict::err()), executionResult)));
}

TEST_F(CustomScorerTests, Scoring_UnknownVerdict) {
    ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResultBuilder().setStandardError("err").build()));
    ON_CALL(testCaseVerdictParser, parseStream(_)).WillByDefault(Throw(runtime_error("bogus")));

    EXPECT_THAT(scorer.score("1.in", "1.out", "eval.out"), Eq(
            ScoringResult(TestCaseVerdict(Verdict::err()), ExecutionResultBuilder()
                    .setStandardError("bogus\nerr").build())));
}

}
