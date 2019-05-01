#include "gmock/gmock.h"
#include "../../../mock.hpp"

#include <sstream>

#include "../../os/MockOperatingSystem.hpp"
#include "../../verdict/MockTestCaseVerdictParser.hpp"
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
    MOCK(TestCaseVerdictParser) testCaseVerdictParser;
    string communicatorCommand = "./communicator";
    EvaluationOptions options;

    Communicator communicator = {&os, &testCaseVerdictParser, communicatorCommand};

    void SetUp() {
        ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResultBuilder().setStandardError("AC\n").build()));
        ON_CALL(os, openForReading(_)).WillByDefault(Return(new istringstream()));
        ON_CALL(testCaseVerdictParser, parseExecutionResult(_)).WillByDefault(Return(optional<TestCaseVerdict>()));
        ON_CALL(testCaseVerdictParser, parseStream(_)).WillByDefault(Return(TestCaseVerdict(Verdict::ac())));
    }
};

TEST_F(CommunicatorTests, Communication_Successful) {
    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));
}

TEST_F(CommunicatorTests, Communication_Crashed) {
    auto executionResult = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("crashed")
            .build();
    ON_CALL(os, execute(_)).WillByDefault(Return(executionResult));
    ON_CALL(testCaseVerdictParser, parseExecutionResult(_))
            .WillByDefault(Return(optional<TestCaseVerdict>(TestCaseVerdict(Verdict::rte()))));

    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(TestCaseVerdict(Verdict::rte()), executionResult)));
}

TEST_F(CommunicatorTests, Communication_UnknownVerdict) {
    ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResultBuilder().setStandardError("err").build()));
    ON_CALL(testCaseVerdictParser, parseStream(_)).WillByDefault(Throw(runtime_error("bogus")));

    EXPECT_THAT(communicator.communicate("1.in", options), Eq(
            CommunicationResult(TestCaseVerdict(Verdict::err()), ExecutionResultBuilder()
                    .setStandardError("bogus").build())));
}

}
