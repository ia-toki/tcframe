#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/generator/GeneratorLogger.hpp"

using ::testing::InSequence;
using ::testing::Test;

namespace tcframe {

class GeneratorLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    GeneratorLogger logger = GeneratorLogger(&engine);
};

TEST_F(GeneratorLoggerTests, Introduction) {
    EXPECT_CALL(engine, logParagraph(0, "Generating test cases..."));

    logger.logIntroduction();
}

TEST_F(GeneratorLoggerTests, Result_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, ""));
        EXPECT_CALL(engine, logParagraph(0, "Generation finished. All test cases OK."));
    }
    logger.logSuccessfulResult();
}

TEST_F(GeneratorLoggerTests, Result_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, ""));
        EXPECT_CALL(engine, logParagraph(0, "Generation finished. Some test cases FAILED."));
    }
    logger.logFailedResult();
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Successful) {
    EXPECT_CALL(engine, logParagraph(0, "OK"));

    logger.logTestCaseSuccessfulResult();
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Description: N = 1"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logTestCaseFailedResult(optional<string>("N = 1"));
}

TEST_F(GeneratorLoggerTests, TestCaseResult_Failed_WithoutDescription) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logTestCaseFailedResult(optional<string>());
}

TEST_F(GeneratorLoggerTests, ConstraintsVerificationFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy constraints, on:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
    }
    ConstraintsVerificationResult result({{-1, {"A <= 10", "B <= 10"}}}, {});
    logger.logConstraintsVerificationFailure(result);
}

TEST_F(GeneratorLoggerTests, SampleTestCaseCheckFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine,
                logListItem1(2, "Sample test case output differs from actual output produced by the solution:"));
        EXPECT_CALL(engine, logListItem2(3, "diff"));
    }
    logger.logSampleTestCaseCheckFailure("diff");
}

TEST_F(GeneratorLoggerTests, ConstraintsVerificationFailure_WithSubtasks) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 2, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 4, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= B"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 1 but is not assigned to it"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 3 but is not assigned to it"));
    }
    ConstraintsVerificationResult result({{2, {"A <= 10", "B <= 10"}}, {4, {"A <= B"}}}, {1, 3});
    logger.logConstraintsVerificationFailure(result);
}

TEST_F(GeneratorLoggerTests, ConstraintsVerificationFailure_WithConstraintsAndSubtasks) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy constraints, on:"));
        EXPECT_CALL(engine, logListItem2(3, "X <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 2, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 1 but is not assigned to it"));
    }
    ConstraintsVerificationResult result({{-1, {"X <= 10"}}, {2, {"A <= 10", "B <= 10"}}}, {1});
    logger.logConstraintsVerificationFailure(result);
}

TEST_F(GeneratorLoggerTests, MultipleTestCasesCombinationIntroduction) {
    EXPECT_CALL(engine, logHangingParagraph(1, "Combining test cases into a single file (foo_3): "));

    logger.logMultipleTestCasesCombinationIntroduction("foo_3");
}

TEST_F(GeneratorLoggerTests, MultipleTestCasesCombinationResult_Successful) {
    EXPECT_CALL(engine, logParagraph(0, "OK"));

    logger.logMultipleTestCasesCombinationSuccessfulResult();
}

TEST_F(GeneratorLoggerTests, MultipleTestCasesCombinationResult_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logParagraph(0, "FAILED"));
        EXPECT_CALL(engine, logParagraph(2, "Reasons:"));
    }
    logger.logMultipleTestCasesCombinationFailedResult();
}

}
