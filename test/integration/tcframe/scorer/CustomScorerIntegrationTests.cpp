#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/scorer/CustomScorer.hpp"

using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class CustomScorerIntegrationTests : public Test {
protected:
    CustomScorer scorer = CustomScorer(new UnixOperatingSystem(), "test-integration/scorer/custom/scorer");

    static void SetUpTestCase() {
        system("g++ -o test-integration/scorer/custom/scorer test-integration/scorer/custom/scorer.cpp");
    }
};

TEST_F(CustomScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = scorer.score(
            "test-integration/scorer/judge.in",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/custom/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::ac()));
}

TEST_F(CustomScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = scorer.score(
            "test-integration/scorer/judge.in",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/custom/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::wa()));
}

TEST_F(CustomScorerIntegrationTests, Scoring_ERR_Crash) {
    ScoringResult result = scorer.score(
            "test-integration/scorer/judge.in",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/custom/contestant_err_crash.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::err()));

    ExecutionResult executionResult = result.executionResult().value();
    EXPECT_FALSE(executionResult.isSuccessful());
    EXPECT_TRUE(executionResult.exitSignal());
}

TEST_F(CustomScorerIntegrationTests, Scoring_ERR_Bogus) {
    ScoringResult result = scorer.score(
            "test-integration/scorer/judge.in",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/custom/contestant_err_bogus.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::err()));
    EXPECT_THAT(result.privateInfo().value(), HasSubstr("Unknown verdict: bogus"));
}

}
