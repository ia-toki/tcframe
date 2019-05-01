#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/scorer/DiffScorer.hpp"

using ::testing::AllOf;
using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class DiffScorerIntegrationTests : public Test {
protected:
    DiffScorer scorer = DiffScorer(new OperatingSystem());
};

TEST_F(DiffScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = scorer.score(
            "",
            "test-integration/runner/evaluator/scorer/judge.out",
            "test-integration/runner/evaluator/scorer/diff/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::ac())));
    EXPECT_THAT(result.executionResult().standardError(), IsEmpty());
}

TEST_F(DiffScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = scorer.score(
            "",
            "test-integration/runner/evaluator/scorer/judge.out",
            "test-integration/runner/evaluator/scorer/diff/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::wa())));

    EXPECT_THAT(result.executionResult().standardError(), AllOf(
            StartsWith("Diff:\n"),
            HasSubstr("(expected) [line 02]    1 2 3\n"),
            HasSubstr("(received) [line 02]    1 4 5\n")));
}

}
