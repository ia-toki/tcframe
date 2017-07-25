#include "gmock/gmock.h"
#include "../../../mock.hpp"

#include <sstream>

#include "../../../os/MockOperatingSystem.hpp"
#include "../../../verdict/MockVerdictCreator.hpp"
#include "tcframe/runner/evaluator/communicator/Communicator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

using std::istringstream;

namespace tcframe {

class CommunicatorTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    MOCK(VerdictCreator) verdictCreator;
    string communicatorCommand = "./communicator";
    EvaluationOptions options;

    Communicator communicator = Communicator(&os, &verdictCreator, communicatorCommand);

    void SetUp() {
        ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResultBuilder().setStandardError("AC\n").build()));
        ON_CALL(os, openForReading(_)).WillByDefault(Return(new istringstream()));
        ON_CALL(verdictCreator, fromExecutionResult(_)).WillByDefault(Return(optional<Verdict>()));
        ON_CALL(verdictCreator, fromStream(_)).WillByDefault(Return(Verdict(VerdictStatus::ac())));
    }
};

TEST_F(CommunicatorTests, Communication_Successful) {
    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(Verdict(VerdictStatus::ac()), ExecutionResult())));
}

TEST_F(CommunicatorTests, Communication_Crashed) {
    ExecutionResult executionResult = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("crashed")
            .build();
    ON_CALL(os, execute(_)).WillByDefault(Return(executionResult));
    ON_CALL(verdictCreator, fromExecutionResult(_))
            .WillByDefault(Return(optional<Verdict>(Verdict(VerdictStatus::rte()))));

    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(Verdict(VerdictStatus::rte()), executionResult)));
}

TEST_F(CommunicatorTests, Communication_UnknownVerdict) {
    ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResultBuilder().setStandardError("err").build()));
    ON_CALL(verdictCreator, fromStream(_)).WillByDefault(Throw(runtime_error("bogus")));

    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(Verdict(VerdictStatus::err()), ExecutionResultBuilder()
                    .setStandardError("bogus").build())));
}

}
