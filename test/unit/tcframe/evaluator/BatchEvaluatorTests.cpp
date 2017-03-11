#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/evaluator/BatchEvaluator.hpp"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class BatchEvaluatorTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    EvaluatorConfig config = EvaluatorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    BatchEvaluator evaluator = BatchEvaluator(&os);
};

TEST_F(BatchEvaluatorTests, Evaluation_OK) {
    EXPECT_CALL(os, execute(AllOf(
            Property(&ExecutionRequest::command, "python Sol.py"),
            Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")),
            Property(&ExecutionRequest::outputFilename, optional<string>("dir/foo_1.out")),
            Property(&ExecutionRequest::timeLimit, optional<int>(3)),
            Property(&ExecutionRequest::memoryLimit, optional<int>(128)))))
            .WillOnce(Return(ExecutionResult()));

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResult()));
}

TEST_F(BatchEvaluatorTests, Evaluation_RTE_ExitCode) {
    ExecutionResult executionResult = ExecutionResultBuilder().setExitCode(1).build();

    EXPECT_CALL(os, execute(_))
            .WillOnce(Return(executionResult));

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResultBuilder()
            .setExecutionResult(executionResult)
            .setVerdict(Verdict::rte())
            .build()));
}

TEST_F(BatchEvaluatorTests, Evaluation_RTE_ExitSignal) {
    ExecutionResult executionResult = ExecutionResultBuilder().setExitSignal("SIGSEGV").build();

    EXPECT_CALL(os, execute(_))
            .WillOnce(Return(executionResult));

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResultBuilder()
            .setExecutionResult(executionResult)
            .setVerdict(Verdict::rte())
            .build()));
}

TEST_F(BatchEvaluatorTests, Evaluation_TLE) {
    ExecutionResult executionResult = ExecutionResultBuilder()
            .setExitSignal("SIGXCPU")
            .setExceededCpuLimits(true)
            .build();

    EXPECT_CALL(os, execute(_))
            .WillOnce(Return(executionResult));

    EXPECT_THAT(evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", config), Eq(EvaluationResultBuilder()
            .setExecutionResult(executionResult)
            .setVerdict(Verdict::tle())
            .build()));
}

}
