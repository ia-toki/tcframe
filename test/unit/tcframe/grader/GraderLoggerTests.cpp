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

TEST_F(GraderLoggerTests, TestCaseGradeSummary) {
    TestCaseGrade grade = TestCaseGradeBuilder()
            .setVerdict(Verdict::ac())
            .build();

    EXPECT_CALL(engine, logParagraph(0, Verdict::ac().name()));

    logger.logTestCaseGradeSummary(grade);
}

TEST_F(GraderLoggerTests, Result) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("RESULT"));
        EXPECT_CALL(engine, logParagraph(1, Verdict::ac().name()));
    }
    logger.logResult(map<int, Verdict>{{Subtask::MAIN_ID, Verdict::ac()}});
}

TEST_F(GraderLoggerTests, Result_WithSubtasks) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("RESULT"));
        EXPECT_CALL(engine, logParagraph(1, "Subtask 1: " + Verdict::ac().name()));
        EXPECT_CALL(engine, logParagraph(1, "Subtask 2: " + Verdict::wa().name()));
    }
    logger.logResult(map<int, Verdict>{
            {1, Verdict::ac()},
            {2, Verdict::wa()}});
}

}
