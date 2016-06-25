#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/generator/GeneratorLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class GeneratorLoggerTests : public Test {
protected:
    Mock(LoggerEngine) engine;

    GeneratorLogger logger = GeneratorLogger(&engine);
};

TEST_F(GeneratorLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Generating test cases..."));

    logger.logIntroduction();
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Successful) {
    EXPECT_CALL(engine, logParagraph(0, "OK"));

    logger.logTestCaseResult("N = 1", TestCaseGenerationResult::successfulResult());
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Failed_Verification) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Description: N = 1"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy constraints, on:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
    }

    VerificationResult result({{-1, {"A <= 10", "B <= 10"}}}, {});
    VerificationFailure failure(result);
    logger.logTestCaseResult("N = 1", TestCaseGenerationResult::failedResult(&failure));
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Failed_Verification_WithGroups) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Description: N = 1"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 2, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 4, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= B"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 1 but is not assigned to it"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 3 but is not assigned to it"));
    }

    VerificationResult result({{2, {"A <= 10", "B <= 10"}}, {4, {"A <= B"}}}, {1, 3});
    VerificationFailure failure(result);
    logger.logTestCaseResult("N = 1", TestCaseGenerationResult::failedResult(&failure));
}

}
