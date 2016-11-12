#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "MockSubmitterLogger.hpp"
#include "tcframe/submitter/BatchEvaluator.hpp"

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
    MOCK(SubmitterLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    SubmitterConfig config = SubmitterConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build();

    BatchEvaluator evaluator = BatchEvaluator(&os, &logger);
};

TEST_F(BatchEvaluatorTests, Evaluation_OK) {
    ON_CALL(os, execute(AllOf(
            Property(&ExecutionRequest::command, "python Sol.py"),
            Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")),
            Property(&ExecutionRequest::timeLimit, optional<int>(3)),
            Property(&ExecutionRequest::memoryLimit, optional<int>(128)))))
            .WillByDefault(Return(ExecutionResult(
                    ExecutionInfoBuilder().setExitCode(0).build(), new istringstream(), new istringstream())));
    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>()));
}

TEST_F(BatchEvaluatorTests, Evaluation_RTE_ExitCode) {
    ON_CALL(os, execute(AllOf(
            Property(&ExecutionRequest::command, "python Sol.py"),
            Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")))))
            .WillByDefault(Return(ExecutionResult(
                    ExecutionInfoBuilder().setExitCode(1).build(), new istringstream(), new istringstream())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::rte()));
        EXPECT_CALL(logger, logSolutionExecutionFailure(_));
    }

    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>(Verdict::rte())));
}

TEST_F(BatchEvaluatorTests, Evaluation_RTE_ExitSignal) {
    ON_CALL(os, execute(AllOf(
            Property(&ExecutionRequest::command, "python Sol.py"),
            Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")))))
            .WillByDefault(Return(ExecutionResult(
                    ExecutionInfoBuilder().setExitSignal("SIGSEGV").build(), new istringstream(), new istringstream())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::rte()));
        EXPECT_CALL(logger, logSolutionExecutionFailure(_));
    }

    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>(Verdict::rte())));
}

TEST_F(BatchEvaluatorTests, Evaluation_TLE) {
    ON_CALL(os, execute(AllOf(
            Property(&ExecutionRequest::command, "python Sol.py"),
            Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")))))
            .WillByDefault(Return(ExecutionResult(
                    ExecutionInfoBuilder()
                            .setExitSignal("SIGXCPU")
                            .setExceededCpuLimits(true)
                            .build(), new istringstream(), new istringstream())));

    EXPECT_CALL(logger, logTestCaseVerdict(Verdict::tle()));

    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>(Verdict::tle())));
}

}
