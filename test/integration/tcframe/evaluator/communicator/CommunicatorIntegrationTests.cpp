#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/evaluator/communicator/Communicator.hpp"

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
            "test-integration/evaluator/communicator/communicator");

    static void SetUpTestCase() {
        system(
                "g++ -o "
                "test-integration/evaluator/communicator/communicator "
                "test-integration/evaluator/communicator/communicator.cpp");
    }
};

TEST_F(CommunicatorIntegrationTests, Communication) {
    system(
            "g++ -o "
            "test-integration/evaluator/communicator/contestant "
            "test-integration/evaluator/communicator/contestant.cpp");

    EvaluationOptions options = EvaluationOptionsBuilder()
            .setSolutionCommand("test-integration/evaluator/communicator/contestant")
            .build();
    CommunicationResult result = communicator.communicate(
            "test-integration/evaluator/communicator/judge.in",
            options);
    EXPECT_THAT(result.verdict(), Eq(Verdict(VerdictStatus::ac())));
    EXPECT_THAT(result.executionResult().standardError(), IsEmpty());
}

}
