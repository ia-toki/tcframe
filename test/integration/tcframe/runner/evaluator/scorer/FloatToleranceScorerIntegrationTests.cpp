#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/scorer/FloatToleranceScorer.hpp"

using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Test;

namespace tcframe {

class FloatToleranceScorerIntegrationTests : public Test {
protected:
    FloatToleranceScorer absoluteScorer = FloatToleranceScorer(new OperatingSystem(), 1e-9,
            FloatToleranceScorer::Mode::ABSOLUTE);
    FloatToleranceScorer relativeScorer = FloatToleranceScorer(new OperatingSystem(), 1e-9,
            FloatToleranceScorer::Mode::RELATIVE);
};

TEST_F(FloatToleranceScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = absoluteScorer.score(
            "",
            "test-integration/runner/evaluator/scorer/float/judge.out",
            "test-integration/runner/evaluator/scorer/float/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::ac())));
    EXPECT_THAT(result.executionResult().standardError(), IsEmpty());
}

TEST_F(FloatToleranceScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = relativeScorer.score(
            "",
            "test-integration/runner/evaluator/scorer/float/judge.out",
            "test-integration/runner/evaluator/scorer/float/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::wa())));
    EXPECT_THAT(result.executionResult().standardError(),
            Eq("Token 3: expected '2.5e3', received '2.6e3'\n"));
}

}
