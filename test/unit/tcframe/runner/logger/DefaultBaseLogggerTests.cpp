#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "MockLoggerEngine.hpp"
#include "tcframe/runner/logger/DefaultBaseLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class DefaultBaseLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    DefaultBaseLogger logger = DefaultBaseLogger(&engine);
};

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Sample) {
    EXPECT_CALL(engine, logHeading("SAMPLE TEST CASES"));
    logger.logTestGroupIntroduction(TestGroup::SAMPLE_ID);
}

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Official) {
    EXPECT_CALL(engine, logHeading("OFFICIAL TEST CASES"));
    logger.logTestGroupIntroduction(TestGroup::MAIN_ID);
}

TEST_F(DefaultBaseLoggerTests, TestGroupIntroduction_Official_WithGroups) {
    EXPECT_CALL(engine, logHeading("TEST GROUP 3"));
    logger.logTestGroupIntroduction(3);
}

TEST_F(DefaultBaseLoggerTests, TestCaseIntroduction) {
    EXPECT_CALL(engine, logHangingParagraph(1, "foo_1: "));
    logger.logTestCaseIntroduction("foo_1");
}


TEST_F(DefaultBaseLoggerTests, Error_Simple) {
    EXPECT_CALL(engine, logListItem1(2, "error"));
    logger.logError(new runtime_error("error"));
}

TEST_F(DefaultBaseLoggerTests, Error_Formatted) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "error 1"));
        EXPECT_CALL(engine, logListItem2(3, "error 1a"));
        EXPECT_CALL(engine, logListItem2(3, "error 1b"));
        EXPECT_CALL(engine, logListItem1(2, "error 2"));
    }
    logger.logError(new FormattedError({
            {0, "error 1"},
            {1, "error 1a"},
            {1, "error 1b"},
            {0, "error 2"}}));
}

}
