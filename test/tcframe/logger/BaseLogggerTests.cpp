#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockLoggerEngine.hpp"
#include "tcframe/logger/BaseLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class BaseLoggerTests : public Test {
protected:
    Mock(LoggerEngine) engine;

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

}
