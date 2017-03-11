#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockLoggerEngine.hpp"
#include "tcframe/logger/BaseLogger.hpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class BaseLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    BaseLogger logger = BaseLogger(&engine);
};

TEST_F(BaseLoggerTests, TestGroupIntroduction_Sample) {
    EXPECT_CALL(engine, logHeading("SAMPLE TEST CASES"));
    logger.logTestGroupIntroduction(0);
}

TEST_F(BaseLoggerTests, TestGroupIntroduction_Official) {
    EXPECT_CALL(engine, logHeading("OFFICIAL TEST CASES"));
    logger.logTestGroupIntroduction(-1);
}

TEST_F(BaseLoggerTests, TestGroupIntroduction_Official_WithGroups) {
    EXPECT_CALL(engine, logHeading("TEST GROUP 3"));
    logger.logTestGroupIntroduction(3);
}

TEST_F(BaseLoggerTests, TestCaseIntroduction) {
    EXPECT_CALL(engine, logHangingParagraph(1, "foo_1: "));
    logger.logTestCaseIntroduction("foo_1");
}

TEST_F(BaseLoggerTests, TestCaseGradeDetails_EvaluationFailure) {
    EXPECT_CALL(engine, logListItem1(2, "Execution of solution failed:"));

    TestCaseGrade grade = TestCaseGradeBuilder()
            .setEvaluationExecutionResult(ExecutionResultBuilder().setExitCode(1).build())
            .build();
    logger.logTestCaseGradeDetails(grade);
}

TEST_F(BaseLoggerTests, TestCaseGradeDetails_ScoringFailure) {
    EXPECT_CALL(engine, logListItem1(2, "Execution of scorer failed:"));

    TestCaseGrade grade = TestCaseGradeBuilder()
            .setScoringExecutionResult(ExecutionResultBuilder().setExitCode(1).build())
            .build();
    logger.logTestCaseGradeDetails(grade);
}

TEST_F(BaseLoggerTests, TestCaseGradeDetails_PrivateScoringInfo) {
    EXPECT_CALL(engine, logListItem1(2, "error"));

    TestCaseGrade grade = TestCaseGradeBuilder()
            .setPrivateScoringInfo("error")
            .build();
    logger.logTestCaseGradeDetails(grade);
}

TEST_F(BaseLoggerTests, ExecutionFailure_ExitCode) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Execution of context failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit code: 1"));
        EXPECT_CALL(engine, logListItem2(3, "Standard error: err"));
    }

    ExecutionResult result = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("err")
            .build();
    logger.logExecutionFailure("context", result);
}

TEST_F(BaseLoggerTests, ExecutionFailure_ExitSignal) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Execution of context failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit signal: SIGSEGV"));
    }

    ExecutionResult result = ExecutionResultBuilder().setExitSignal("SIGSEGV").build();
    logger.logExecutionFailure("context", result);
}

}
