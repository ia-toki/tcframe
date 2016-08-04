#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "MockSubmitterLogger.hpp"
#include "tcframe/submitter/BatchEvaluator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class BatchEvaluatorTests : public Test {
protected:
    Mock(OperatingSystem) os;
    Mock(SubmitterLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    SubmitterConfig config = SubmitterConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();

    BatchEvaluator evaluator = BatchEvaluator(&os, &logger);
};

TEST_F(BatchEvaluatorTests, Evaluation_OK) {
    ON_CALL(os, execute("python Sol.py", "dir/foo_1.in", _, _))
            .WillByDefault(Return(ExecutionResult(ExecutionInfo(0), new istringstream(), new istringstream())));
    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>()));
}

TEST_F(BatchEvaluatorTests, Evaluation_RTE) {
    ON_CALL(os, execute("python Sol.py", "dir/foo_1.in", _, _))
            .WillByDefault(Return(ExecutionResult(ExecutionInfo(1), new istringstream(), new istringstream())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseVerdict(Verdict::rte()));
        EXPECT_CALL(logger, logSolutionExecutionFailure(_));
    }

    EXPECT_THAT(evaluator.evaluate(testCase, config), Eq(optional<Verdict>(Verdict::rte())));
}

}
