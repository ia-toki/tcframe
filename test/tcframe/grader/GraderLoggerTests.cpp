#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/grader/GraderLogger.hpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class GraderLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    GraderLogger logger = GraderLogger(&engine);
};

TEST_F(GraderLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Grading..."));

    logger.logIntroduction();
}

TEST_F(GraderLoggerTests, TestCaseVerdict) {
    EXPECT_CALL(engine, logParagraph(0, Verdict::ac().name()));

    logger.logTestCaseVerdict(Verdict::ac());
}

TEST_F(GraderLoggerTests, TestCaseScoringMessage) {
    EXPECT_CALL(engine, logListItem1(2, "lorem"));
    logger.logTestCaseScoringMessage("lorem");
}

TEST_F(GraderLoggerTests, TestCaseScoringMessage_Empty) {
    EXPECT_CALL(engine, logListItem1(_, _)).Times(0);
    logger.logTestCaseScoringMessage("");
}

TEST_F(GraderLoggerTests, Result) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("RESULT"));
        EXPECT_CALL(engine, logParagraph(1, Verdict::ac().name()));
    }
    logger.logResult(map<int, Verdict>{{-1, Verdict::ac()}});
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
