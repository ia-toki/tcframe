#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/os.hpp"
#include "tcframe/scorer/DiffScorer.hpp"
#include "tcframe/util.hpp"

using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class DiffScorerIntegrationTests : public Test {
protected:
    DiffScorer scorer = DiffScorer(new UnixOperatingSystem());
};

TEST_F(DiffScorerIntegrationTests, Scoring_AC) {
    ScoringResult result = scorer.score(
            "",
            "integration/scorer/judge.out",
            "integration/scorer/diff/contestant_ac.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::ac()));
}

TEST_F(DiffScorerIntegrationTests, Scoring_WA) {
    ScoringResult result = scorer.score(
            "",
            "integration/scorer/judge.out",
            "integration/scorer/diff/contestant_wa.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict::wa()));

    string scorerMessage = StringUtils::streamToString(result.executionResult().errorStream());
    EXPECT_THAT(scorerMessage, StartsWith("Diff:\n"));
    EXPECT_THAT(scorerMessage, HasSubstr("(expected) [line 02]    1 2 3\n"));
    EXPECT_THAT(scorerMessage, HasSubstr("(received) [line 02]    1 4 5\n"));
}

}
