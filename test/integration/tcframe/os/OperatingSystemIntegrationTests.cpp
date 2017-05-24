#include "gmock/gmock.h"

#include <cstdlib>

#include "tcframe/os/OperatingSystem.hpp"
#include "tcframe/util.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class OperatingSystemIntegrationTests : public Test {
protected:
    OperatingSystem os;

    static string readFile(const string& path) {
        ifstream in(path);
        string contents = StringUtils::streamToString(&in);
        in.close();
        return contents;
    }
};

TEST_F(OperatingSystemIntegrationTests, Execution_Successful) {
    system(
            "g++ -o "
            "test-integration/os/program "
            "test-integration/os/program.cpp");

    ExecutionResult result = os.execute(ExecutionRequestBuilder()
            .setCommand("test-integration/os/program")
            .setInputFilename("test-integration/os/1.in")
            .setOutputFilename("test-integration/os/1.out")
            .build());

    EXPECT_TRUE(result.isSuccessful());
    EXPECT_THAT(result.exitCode(), Eq(optional<int>(0)));
    EXPECT_FALSE(result.exitSignal());

    EXPECT_THAT(readFile("test-integration/os/1.out"), Eq("43\n"));
    EXPECT_THAT(result.standardError(), Eq("44\n"));
}

TEST_F(OperatingSystemIntegrationTests, Execution_ExceededTimeLimits) {
    system(
            "g++ -o "
            "test-integration/os/program_tl "
            "test-integration/os/program_tl.cpp");

    ExecutionResult result = os.execute(ExecutionRequestBuilder()
            .setCommand("test-integration/os/program_tl")
            .setTimeLimit(1)
            .build());

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_FALSE(result.exitCode());
    EXPECT_THAT(result.exitSignal(), Eq(optional<int>(SIGXCPU)));
}

#ifndef __APPLE__

TEST_F(OperatingSystemIntegrationTests, Execution_ExceededMemoryLimits) {
    system(
            "g++ -o "
            "test-integration/os/program_ml "
            "test-integration/os/program_ml.cpp");

    ExecutionResult result = os.execute(ExecutionRequestBuilder()
            .setCommand("test-integration/os/program_ml")
            .setMemoryLimit(16)
            .build());

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_FALSE(result.exitCode());
    EXPECT_THAT(result.exitSignal(), Eq(optional<int>(SIGKILL)));
}

#endif

TEST_F(OperatingSystemIntegrationTests,  Execution_Crashed_ExitCode) {
    system(
            "g++ -o "
            "test-integration/os/program_exit-code "
            "test-integration/os/program_exit-code.cpp");

    ExecutionResult result = os.execute(ExecutionRequestBuilder()
            .setCommand("test-integration/os/program_exit-code")
            .build());

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_THAT(result.exitCode(), Eq(optional<int>(42)));
    EXPECT_FALSE(result.exitSignal());
}

TEST_F(OperatingSystemIntegrationTests, Execution_Crashed_ExitSignal) {
    system(
            "g++ -o "
            "test-integration/os/program_exit-signal "
            "test-integration/os/program_exit-signal.cpp");

    ExecutionResult result = os.execute(ExecutionRequestBuilder()
            .setCommand("test-integration/os/program_exit-signal")
            .build());

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_FALSE(result.exitCode());
    EXPECT_THAT(result.exitSignal(), Eq(optional<int>(SIGFPE)));
}

}
