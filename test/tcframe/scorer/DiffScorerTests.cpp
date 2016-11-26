#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/scorer/DiffScorer.hpp"

using ::testing::Eq;
using ::testing::Property;
using ::testing::StartsWith;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class DiffScorerTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    DiffScorer scorer = DiffScorer(&os);
};

TEST_F(DiffScorerTests, Scoring_AC) {
    ON_CALL(os, execute(Property(&ExecutionRequest::command, StartsWith("diff --brief"))))
            .WillByDefault(Return(ExecutionResultBuilder()
                    .setInfo(ExecutionInfoBuilder().setExitCode(0).build())
                    .build()));

    ScoringResult expectedResult = ScoringResultBuilder().setVerdict(Verdict::ac()).build();
    EXPECT_THAT(scorer.score("input", "output", "evaluation"), Eq(expectedResult));
}

TEST_F(DiffScorerTests, Scoring_WA) {
    ON_CALL(os, execute(Property(&ExecutionRequest::command, StartsWith("diff --brief"))))
            .WillByDefault(Return(ExecutionResultBuilder()
                    .setInfo(ExecutionInfoBuilder().setExitCode(1).build())
                    .build()));
    ON_CALL(os, execute(Property(&ExecutionRequest::command, StartsWith("diff --unchanged"))))
            .WillByDefault(Return(ExecutionResultBuilder()
                    .setInfo(ExecutionInfoBuilder().setExitCode(0).build())
                    .setOutputStream(new istringstream("diff"))
                    .build()));

    ScoringResult expectedResult = ScoringResultBuilder()
            .setVerdict(Verdict::wa())
            .setMessage("Diff:\ndiff")
            .build();
    EXPECT_THAT(scorer.score("input", "output", "evaluation"), Eq(expectedResult));
}

}
