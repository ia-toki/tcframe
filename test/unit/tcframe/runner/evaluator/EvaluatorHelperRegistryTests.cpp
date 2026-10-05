#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>
#include <stdexcept>

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/evaluator/EvaluatorHelperRegistry.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class EvaluatorHelperRegistryTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    EvaluatorHelperRegistry registry;
};

TEST_F(EvaluatorHelperRegistryTests, Scorer_NoCommandNoArgs_DiffScorer) {
    Scorer* scorer = registry.getScorer(&os, optional<string>(), "");

    EXPECT_NE(dynamic_cast<DiffScorer*>(scorer), nullptr);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_CommandWinsOverFloatArgs) {
    Scorer* scorer = registry.getScorer(&os, optional<string>("./scorer"), "float_absolute_tolerance 1e-9");

    EXPECT_NE(dynamic_cast<CustomScorer*>(scorer), nullptr);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_AbsoluteArgs_FloatToleranceScorer) {
    Scorer* scorer = registry.getScorer(&os, optional<string>(), "float_absolute_tolerance 1e-9");

    EXPECT_NE(dynamic_cast<FloatToleranceScorer*>(scorer), nullptr);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_BothArgs_FloatToleranceScorer) {
    Scorer* scorer = registry.getScorer(&os, optional<string>(),
            "float_absolute_tolerance 1e-9 float_relative_tolerance 1e-6");

    EXPECT_NE(dynamic_cast<FloatToleranceScorer*>(scorer), nullptr);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_IgnoredOptionsOnly_DiffScorer) {
    Scorer* scorer = registry.getScorer(&os, optional<string>(), "ignore_whitespace");

    EXPECT_NE(dynamic_cast<DiffScorer*>(scorer), nullptr);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_BadNumber_Throws) {
    EXPECT_THROW(registry.getScorer(&os, optional<string>(), "float_absolute_tolerance abc"), runtime_error);
    EXPECT_THROW(registry.getScorer(&os, optional<string>(), "float_relative_tolerance"), runtime_error);
}

TEST_F(EvaluatorHelperRegistryTests, Scorer_AbsoluteArgs_ScoresWithTolerance) {
    ON_CALL(os, openForReading("judge.out")).WillByDefault(Return(new istringstream("0.5\n")));
    ON_CALL(os, openForReading("contestant.out")).WillByDefault(Return(new istringstream("0.5000000001\n")));
    Scorer* scorer = registry.getScorer(&os, optional<string>(), "float_absolute_tolerance 1e-9");

    EXPECT_THAT(scorer->score("1.in", "judge.out", "contestant.out").verdict(),
            Eq(TestCaseVerdict(Verdict::ac())));
}

}
