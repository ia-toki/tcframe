#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/runner/grader/DefaultGraderLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class DefaultGraderLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    DefaultGraderLogger logger = DefaultGraderLogger(&engine);
};

TEST_F(DefaultGraderLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Local grading with solution command: './solution'..."));

    logger.logIntroduction("./solution");
}

TEST_F(DefaultGraderLoggerTests, TestCaseVerdict) {
    EXPECT_CALL(engine, logParagraph(0, Verdict::ac().name()));

    logger.logTestCaseVerdict(TestCaseVerdict(Verdict::ac()));
}

TEST_F(DefaultGraderLoggerTests, Result) {
    SubtaskVerdict verdict(Verdict::ac(), 100);
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("VERDICT"));
        EXPECT_CALL(engine, logParagraph(1, verdict.toString()));
    }
    logger.logResult({{Subtask::MAIN_ID, verdict}}, verdict);
}

TEST_F(DefaultGraderLoggerTests, Result_WithSubtasks) {
    SubtaskVerdict subtask1Verdict(Verdict::ac(), 70);
    SubtaskVerdict subtask2Verdict(Verdict::wa(), 0);
    SubtaskVerdict verdict(Verdict::wa(), 70);
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("SUBTASK VERDICTS"));
        EXPECT_CALL(engine, logParagraph(1, "Subtask 1: " + subtask1Verdict.toString()));
        EXPECT_CALL(engine, logParagraph(1, "Subtask 2: " + subtask2Verdict.toString()));
        EXPECT_CALL(engine, logHeading("VERDICT"));
        EXPECT_CALL(engine, logParagraph(1, verdict.toString()));
    }
    logger.logResult({
            {1, subtask1Verdict},
            {2, subtask2Verdict}},
            verdict);
}

}
