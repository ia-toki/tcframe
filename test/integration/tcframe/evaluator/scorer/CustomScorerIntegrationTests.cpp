#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/evaluator/scorer/CustomScorer.hpp"

using ::testing::AllOf;
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

TEST_F(CustomScorerIntegrationTests, Scoring) {
    ScoringResult result = scorer.score(
            "test-integration/evaluator/scorer/judge.in",
            "test-integration/evaluator/scorer/judge.out",
            "test-integration/evaluator/scorer/custom/contestant.out");
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::ac())));
}

}
