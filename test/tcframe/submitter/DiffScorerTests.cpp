#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "MockSubmitterLogger.hpp"
#include "tcframe/submitter/DiffScorer.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::StartsWith;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class DiffScorerTests : public Test {
protected:
    Mock(OperatingSystem) os;
    Mock(SubmitterLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    SubmitterConfig config = SubmitterConfigBuilder()
            .setSlug("foo")
            .setTestCasesDir("dir")
            .build();

    DiffScorer scorer = DiffScorer(&os, &logger);
};

TEST_F(DiffScorerTests, Scoring_AC) {
    ON_CALL(os, execute(StartsWith("diff --brief"), _, _, _))
            .WillByDefault(Return(ExecutionResult(ExecutionInfo(0), new istringstream(), new istringstream())));

    EXPECT_CALL(logger, logTestCaseVerdict(Verdict::ac()));
    EXPECT_THAT(scorer.score(testCase, config), Eq(Verdict::ac()));
}

TEST_F(DiffScorerTests, Scoring_WA) {
    ON_CALL(os, execute(StartsWith("diff --brief"), _, _, _))
            .WillByDefault(Return(ExecutionResult(ExecutionInfo(1), new istringstream(), new istringstream())));
    ON_CALL(os, execute(StartsWith("diff --unchanged"), _, _, _))
            .WillByDefault(Return(ExecutionResult(ExecutionInfo(0), new istringstream("diff"), new istringstream())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::wa()));
        EXPECT_CALL(logger, logDiffFailure("diff"));
    }
    EXPECT_THAT(scorer.score(testCase, config), Eq(Verdict::wa()));
}

}
