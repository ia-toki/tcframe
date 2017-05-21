#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/evaluator/scorer/CustomScorer.hpp"

using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class CustomScorerIntegrationTests : public Test {
protected:
    CustomScorer scorer = CustomScorer(
            new UnixOperatingSystem(),
            new VerdictCreator(),
            "test-integration/evaluator/scorer/custom/scorer");

    static void SetUpTestCase() {
        system(
                "g++ -o "
                "test-integration/evaluator/scorer/custom/scorer "
                "test-integration/evaluator/scorer/custom/scorer.cpp");
    }
};

TEST_F(CustomScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = scorer.score(
            "test-integration/evaluator/scorer/judge.in",
            "test-integration/evaluator/scorer/judge.out",
            "test-integration/evaluator/scorer/custom/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::ac())));
}

TEST_F(CustomScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = scorer.score(
            "test-integration/evaluator/scorer/judge.in",
            "test-integration/evaluator/scorer/judge.out",
            "test-integration/evaluator/scorer/custom/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::wa())));
}

TEST_F(CustomScorerIntegrationTests, Scoring_ERR_Crash) {
    ScoringResult result = scorer.score(
            "test-integration/evaluator/scorer/judge.in",
            "test-integration/evaluator/scorer/judge.out",
            "test-integration/evaluator/scorer/custom/contestant_err_crash.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::err())));

    ExecutionResult executionResult = result.executionResult();
    EXPECT_FALSE(executionResult.isSuccessful());
    EXPECT_TRUE(executionResult.exitSignal());
}

TEST_F(CustomScorerIntegrationTests, Scoring_ERR_Bogus) {
    ScoringResult result = scorer.score(
            "test-integration/evaluator/scorer/judge.in",
            "test-integration/evaluator/scorer/judge.out",
            "test-integration/evaluator/scorer/custom/contestant_err_bogus.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::err())));

    ExecutionResult executionResult = result.executionResult();
    EXPECT_TRUE(executionResult.isSuccessful());
    EXPECT_THAT(executionResult.standardError(), HasSubstr("Unknown verdict: bogus"));
}

}
