#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/submitter/SubmitterLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class SubmitterLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    SubmitterLogger logger = SubmitterLogger(&engine);
};

TEST_F(SubmitterLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Submitting..."));

    logger.logIntroduction();
}

TEST_F(SubmitterLoggerTests, TestCaseVerdict) {
    EXPECT_CALL(engine, logParagraph(0, Verdict::ac().name()));

    logger.logTestCaseVerdict(Verdict::ac());
}

TEST_F(SubmitterLoggerTests, DiffFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Diff:"));
        EXPECT_CALL(engine, logParagraph(0, "lorem"));
    }
    logger.logDiffFailure("lorem");
}

TEST_F(SubmitterLoggerTests, Result) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("RESULT"));
        EXPECT_CALL(engine, logParagraph(1, Verdict::ac().name()));
    }
    logger.logResult(map<int, Verdict>{{-1, Verdict::ac()}});
}

TEST_F(SubmitterLoggerTests, Result_WithSubtasks) {
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
