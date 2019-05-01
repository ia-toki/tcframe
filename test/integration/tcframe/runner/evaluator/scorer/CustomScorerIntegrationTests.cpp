#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/runner/evaluator/scorer/CustomScorer.hpp"

using ::testing::AllOf;
using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::Test;

namespace tcframe {

class CustomScorerIntegrationTests : public Test {
protected:
    CustomScorer scorer = {
            new OperatingSystem(),
            new TestCaseVerdictParser(),
            "test-integration/runner/evaluator/scorer/custom/scorer"};

    static void SetUpTestCase() {
        system(
                "g++ -o "
                "test-integration/runner/evaluator/scorer/custom/scorer "
                "test-integration/runner/evaluator/scorer/custom/scorer.cpp");
    }
};

TEST_F(CustomScorerIntegrationTests, Scoring) {
    ScoringResult result = scorer.score(
            "test-integration/runner/evaluator/scorer/judge.in",
            "test-integration/runner/evaluator/scorer/judge.out",
            "test-integration/runner/evaluator/scorer/custom/contestant.out");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::ac())));
}

}
