#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/grader/GraderLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class GraderLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    GraderLogger logger = GraderLogger(&engine);
};

TEST_F(GraderLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Local grading with solution command: './solution'..."));

    logger.logIntroduction("./solution");
}

TEST_F(GraderLoggerTests, TestCaseVerdict) {
    EXPECT_CALL(engine, logParagraph(0, VerdictStatus::ac().name()));

    logger.logTestCaseVerdict(Verdict(VerdictStatus::ac()));
}

TEST_F(GraderLoggerTests, Result) {
    Verdict verdict(VerdictStatus::ac());
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("VERDICT"));
        EXPECT_CALL(engine, logParagraph(1, verdict.toString()));
    }
    logger.logResult({{Subtask::MAIN_ID, verdict}}, verdict);
}

TEST_F(GraderLoggerTests, Result_WithSubtasks) {
    Verdict subtask1Verdict(VerdictStatus::ac(), 70);
    Verdict subtask2Verdict(VerdictStatus::wa(), 0);
    Verdict verdict(VerdictStatus::wa(), 70);
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
