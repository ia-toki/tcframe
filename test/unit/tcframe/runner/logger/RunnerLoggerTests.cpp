#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../logger/MockLoggerEngine.hpp"
#include "tcframe/runner/logger/RunnerLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class RunnerLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    RunnerLogger logger = RunnerLogger(&engine);
};

TEST_F(RunnerLoggerTests, SpecificationFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logHeading("SPECIFICATIONS"));
        EXPECT_CALL(engine, logParagraph(1, "The specifications contain errors:"));
        EXPECT_CALL(engine, logListItem1(1, "lorem ipsum"));
        EXPECT_CALL(engine, logListItem1(1, "dolor sit amet"));
    }
    logger.logSpecificationFailure({"lorem ipsum", "dolor sit amet"});
}

}
