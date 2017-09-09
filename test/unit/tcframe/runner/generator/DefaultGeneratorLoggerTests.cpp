#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../logger/MockLoggerEngine.hpp"
#include "tcframe/runner/generator/DefaultGeneratorLogger.hpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class DefaultGeneratorLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    DefaultGeneratorLogger logger = DefaultGeneratorLogger(&engine);
};

TEST_F(DefaultGeneratorLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Generating test cases..."));

    logger.logIntroduction();
}

TEST_F(DefaultGeneratorLoggerTests, Result_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, ""));
        EXPECT_CALL(engine, logParagraph(0, "Generation finished. All test cases OK."));
    }
    logger.logSuccessfulResult();
}

TEST_F(DefaultGeneratorLoggerTests, Result_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, ""));
        EXPECT_CALL(engine, logParagraph(0, "Generation finished. Some test cases FAILED."));
    }
    logger.logFailedResult();
}

TEST_F(DefaultGeneratorLoggerTests, TestCaseResult_Successful) {
    EXPECT_CALL(engine, logParagraph(0, "OK"));

    logger.logTestCaseSuccessfulResult();
}

TEST_F(DefaultGeneratorLoggerTests, TestCaseResult_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Description: N = 1"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logTestCaseFailedResult(optional<string>("N = 1"));
}

TEST_F(DefaultGeneratorLoggerTests, TestCaseResult_Failed_WithoutDescription) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logTestCaseFailedResult(optional<string>());
}

TEST_F(DefaultGeneratorLoggerTests, MultipleTestCasesCombinationIntroduction) {
    EXPECT_CALL(engine, logHangingParagraph(1, "Combining test cases into a single file (foo_3): "));

    logger.logMultipleTestCasesCombinationIntroduction("foo_3");
}

TEST_F(DefaultGeneratorLoggerTests, MultipleTestCasesCombinationResult_Successful) {
    EXPECT_CALL(engine, logParagraph(0, "OK"));

    logger.logMultipleTestCasesCombinationSuccessfulResult();
}

TEST_F(DefaultGeneratorLoggerTests, MultipleTestCasesCombinationResult_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logMultipleTestCasesCombinationFailedResult();
}

TEST_F(DefaultGeneratorLoggerTests, SimpleError) {
    EXPECT_CALL(engine, logListItem1(2, "error"));
    logger.logSimpleError(runtime_error("error"));
}

TEST_F(DefaultGeneratorLoggerTests, FormattedError) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "error 1"));
        EXPECT_CALL(engine, logListItem2(3, "error 1a"));
        EXPECT_CALL(engine, logListItem2(3, "error 1b"));
        EXPECT_CALL(engine, logListItem1(2, "error 2"));
    }
    logger.logFormattedError(FormattedError({
            {0, "error 1"},
            {1, "error 1a"},
            {1, "error 1b"},
            {0, "error 2"}}));
}

}
