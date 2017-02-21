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

TEST_F(BaseLoggerTests, TestCaseScoringMessage) {
    EXPECT_CALL(engine, logListItem1(2, "lorem"));
    logger.logTestCaseScoringMessage("lorem");
}

TEST_F(BaseLoggerTests, TestCaseScoringMessage_Empty) {
    EXPECT_CALL(engine, logListItem1(_, _)).Times(0);
    logger.logTestCaseScoringMessage("");
}

TEST_F(BaseLoggerTests, ExecutionFailure_ExitCode) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Execution of solution failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit code: 1"));
        EXPECT_CALL(engine, logListItem2(3, "Standard error: err"));
    }

    ExecutionResult result = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("err")
            .build();
    logger.logExecutionFailure("solution", result);
}

TEST_F(BaseLoggerTests, ExecutionFailure_ExitSignal) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Execution of solution failed:"));
        EXPECT_CALL(engine, logListItem2(3, "Exit signal: SIGSEGV"));
    }

    ExecutionResult result = ExecutionResultBuilder().setExitSignal("SIGSEGV").build();
    logger.logExecutionFailure("solution", result);
}

}
