#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../logger/MockLoggerEngine.hpp"
#include "tcframe/runner/grader/BriefGraderLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class BriefGraderLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    BriefGraderLogger logger = BriefGraderLogger(&engine);
};

TEST_F(BriefGraderLoggerTests, Result) {
    Verdict verdict(VerdictStatus::ac());
    EXPECT_CALL(engine, logParagraph(0, verdict.toBriefString()));
    logger.logResult({{Subtask::MAIN_ID, verdict}}, verdict);
}

TEST_F(BriefGraderLoggerTests, Result_WithSubtasks) {
    Verdict verdict(VerdictStatus::wa(), 70);
    Verdict subtask1Verdict(VerdictStatus::ac(), 70);
    Verdict subtask2Verdict(VerdictStatus::wa(), 0);
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, verdict.toBriefString()));
        EXPECT_CALL(engine, logParagraph(0, subtask1Verdict.toBriefString()));
        EXPECT_CALL(engine, logParagraph(0, subtask2Verdict.toBriefString()));
    }
    logger.logResult({{1, subtask1Verdict}, {2, subtask2Verdict}}, verdict);
}

}
