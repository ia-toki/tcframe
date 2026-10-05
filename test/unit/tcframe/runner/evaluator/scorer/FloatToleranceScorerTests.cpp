#include "gmock/gmock.h"
#include "../../../mock.hpp"

#include <sstream>

#include "../../os/MockOperatingSystem.hpp"
#include "tcframe/runner/evaluator/scorer/FloatToleranceScorer.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class FloatToleranceScorerTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    ScoringResult score(FloatToleranceScorer::Mode mode, double eps, const string& expected, const string& received) {
        ON_CALL(os, openForReading("judge.out")).WillByDefault(Return(new istringstream(expected)));
        ON_CALL(os, openForReading("contestant.out")).WillByDefault(Return(new istringstream(received)));
        FloatToleranceScorer scorer(&os, eps, mode);
        return scorer.score("1.in", "judge.out", "contestant.out");
    }
};

TEST_F(FloatToleranceScorerTests, Scoring_IdenticalOutput_AC) {
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-9, "1 2.5 abc\n", "1 2.5 abc\n"), Eq(
            ScoringResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));
}

TEST_F(FloatToleranceScorerTests, Scoring_AbsoluteWithinEps_AC) {
    EXPECT_THAT(score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-6, "0.5\n", "0.5000001\n"), Eq(
            ScoringResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));
}

TEST_F(FloatToleranceScorerTests, Scoring_AbsoluteOutsideEps_WA) {
    auto result = score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-6, "0.5\n", "0.501\n");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::wa())));
    EXPECT_THAT(result.executionResult().standardError(),
            Eq("Token 1: expected '0.5', received '0.501'\n"));
}

TEST_F(FloatToleranceScorerTests, Scoring_RelativeScalesWithMagnitude_AC) {
    // Absolute difference is 1, but relative difference is 1e-9.
    EXPECT_THAT(score(FloatToleranceScorer::Mode::RELATIVE, 1e-8, "1000000000\n", "1000000001\n").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
    EXPECT_THAT(score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-8, "1000000000\n", "1000000001\n").verdict(),
            Eq(TestCaseVerdict(Verdict::wa())));
}

TEST_F(FloatToleranceScorerTests, Scoring_BothAcceptsEitherTolerance) {
    // Passes absolute check only.
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-6, "0\n", "1e-7\n").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
    // Passes relative check only.
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-8, "1000000000\n", "1000000001\n").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
    // Passes neither.
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-8, "1\n", "2\n").verdict(),
            Eq(TestCaseVerdict(Verdict::wa())));
}

TEST_F(FloatToleranceScorerTests, Scoring_BothWithSeparateEps) {
    ON_CALL(os, openForReading("judge.out")).WillByDefault(Return(new istringstream("1000000000\n")));
    ON_CALL(os, openForReading("contestant.out")).WillByDefault(Return(new istringstream("1000000001\n")));

    // Absolute eps 2 accepts a difference of 1; relative eps 1e-12 would not.
    FloatToleranceScorer scorer(&os, 2, 1e-12, FloatToleranceScorer::Mode::BOTH);
    EXPECT_THAT(scorer.score("1.in", "judge.out", "contestant.out").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));

    // The mock streams are consumed by the first score() call, so re-arm them.
    ON_CALL(os, openForReading("judge.out")).WillByDefault(Return(new istringstream("1000000000\n")));
    ON_CALL(os, openForReading("contestant.out")).WillByDefault(Return(new istringstream("1000000001\n")));
    FloatToleranceScorer relativeOnly(&os, 2, 1e-12, FloatToleranceScorer::Mode::RELATIVE);
    EXPECT_THAT(relativeOnly.score("1.in", "judge.out", "contestant.out").verdict(),
            Eq(TestCaseVerdict(Verdict::wa())));
}

TEST_F(FloatToleranceScorerTests, Scoring_NonFloatTokensMustMatchExactly) {
    auto result = score(FloatToleranceScorer::Mode::BOTH, 1e-6, "1.0 yes\n", "1.0 no\n");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::wa())));
    EXPECT_THAT(result.executionResult().standardError(),
            Eq("Token 2: expected 'yes', received 'no'\n"));
}

TEST_F(FloatToleranceScorerTests, Scoring_FloatVersusNonFloat_WA) {
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-6, "1\n", "x\n").verdict(),
            Eq(TestCaseVerdict(Verdict::wa())));
    EXPECT_THAT(score(FloatToleranceScorer::Mode::BOTH, 1e-6, "nan\n", "inf\n").verdict(),
            Eq(TestCaseVerdict(Verdict::wa())));
}

TEST_F(FloatToleranceScorerTests, Scoring_IntegerAndFloatSpelling_AC) {
    EXPECT_THAT(score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-9, "1\n", "1.0\n").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
}

TEST_F(FloatToleranceScorerTests, Scoring_WhitespaceLayoutIgnored_AC) {
    EXPECT_THAT(score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-9, "1 2\n3\n", "1\t2 3").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
}

TEST_F(FloatToleranceScorerTests, Scoring_TokenCountMismatch_WA) {
    auto result = score(FloatToleranceScorer::Mode::ABSOLUTE, 1e-9, "1 2\n", "1\n");
    EXPECT_THAT(result.verdict(), Eq(TestCaseVerdict(Verdict::wa())));
    EXPECT_THAT(result.executionResult().standardError(),
            Eq("Token count: expected 2, received 1\n"));
}

}
