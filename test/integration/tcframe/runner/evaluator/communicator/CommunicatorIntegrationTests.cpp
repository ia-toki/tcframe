#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/runner/evaluator/communicator/Communicator.hpp"

using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::IsEmpty;
using ::testing::Test;

namespace tcframe {

class CommunicatorIntegrationTests : public Test {
protected:
    Communicator communicator = Communicator(
            new OperatingSystem(),
            new VerdictCreator(),
            "test-integration/runner/evaluator/communicator/communicator");

    static void SetUpTestCase() {
        system(
                "g++ -o "
                "test-integration/runner/evaluator/communicator/communicator "
                "test-integration/runner/evaluator/communicator/communicator.cpp");
    }
};

TEST_F(CommunicatorIntegrationTests, Communication) {
    system(
            "g++ -o "
            "test-integration/runner/evaluator/communicator/contestant "
            "test-integration/runner/evaluator/communicator/contestant.cpp");

    EvaluationOptions options = EvaluationOptionsBuilder()
            .setSolutionCommand("test-integration/runner/evaluator/communicator/contestant")
            .build();
    CommunicationResult result = communicator.communicate(
            "test-integration/runner/evaluator/communicator/judge.in",
            options);
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::ac())));
    EXPECT_THAT(result.executionResult().standardError(), IsEmpty());
}

}
