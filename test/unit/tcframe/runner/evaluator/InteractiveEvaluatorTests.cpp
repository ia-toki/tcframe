#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "communicator/MockCommunicator.hpp"
#include "tcframe/runner/evaluator/InteractiveEvaluator.hpp"

using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class InteractiveEvaluatorTests : public Test {
protected:
    MOCK(Communicator) communicator;

    InteractiveEvaluator evaluator = InteractiveEvaluator(&communicator);
};

TEST_F(InteractiveEvaluatorTests, Evaluation) {
    Verdict verdict(VerdictStatus::wa());
    auto executionResult = ExecutionResultBuilder().setStandardError("WA").build();
    auto options = EvaluationOptionsBuilder().setSolutionCommand("\"python Sol.py\"").build();

    EXPECT_CALL(communicator, communicate("1.in", options))
                .WillOnce(Return(CommunicationResult(verdict, executionResult)));
    EXPECT_THAT(evaluator.evaluate("1.in", "", options), Eq(
            EvaluationResult(verdict, {{"communicator", executionResult}})));
}

}
