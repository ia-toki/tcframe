#include "gmock/gmock.h"

#include "tcframe/scorer/DiffScorer.hpp"

using ::testing::AllOf;
using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class DiffScorerIntegrationTests : public Test {
protected:
    DiffScorer scorer = DiffScorer(new UnixOperatingSystem());
};

TEST_F(DiffScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = scorer.score(
            "",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/diff/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::ac()));
    EXPECT_FALSE(result.privateInfo());
}

TEST_F(DiffScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = scorer.score(
            "",
            "test-integration/scorer/judge.out",
            "test-integration/scorer/diff/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::wa()));

    EXPECT_THAT(result.privateInfo().value(), AllOf(
            StartsWith("Diff:\n"),
            HasSubstr("(expected) [line 02]    1 2 3\n"),
            HasSubstr("(received) [line 02]    1 4 5\n")));
}

}
