#include "gmock/gmock.h"

#include "tcframe/runner/os/ExecutionResult.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class ExecutionResultTests : public Test {};

TEST_F(ExecutionResultTests, AsFormattedError) {
    map<string, ExecutionResult> executionResults = {
            {"solution", ExecutionResultBuilder().setExitCode(1).setStandardError("err").build()},
            {"scorer1", ExecutionResultBuilder().setExitCode(0).setStandardError("diff: abc").build()},
            {"scorer2", ExecutionResultBuilder().setExitSignal(SIGSEGV).build()}};

    FormattedError error = ExecutionResults::asFormattedError(executionResults);
    EXPECT_THAT(error.messages(), Eq(vector<pair<int, string>>{
            {0, "scorer1: diff: abc"},
            {0, "Execution of scorer2 failed:"},
            {1, "Exit signal: " + string(strsignal(SIGSEGV))},
            {0, "Execution of solution failed:"},
            {1, "Exit code: 1"},
            {1, "Standard error: err"}}));
}

}
