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

TEST_F(DefaultGeneratorLoggerTests, ConstraintsVerificationFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy constraints, on:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
    }
    ConstraintsVerificationResult result({{Subtask::MAIN_ID, {"A <= 10", "B <= 10"}}}, {});
    logger.logConstraintsVerificationFailure(result);
}

TEST_F(DefaultGeneratorLoggerTests, SampleTestCaseCheckFailure) {
    {
        InSequence sequence;
        EXPECT_CALL(engine,
                logListItem1(2, "Sample test case output does not match with actual output produced by the solution"));
    }
    logger.logSampleTestCaseCheckFailure();
}

TEST_F(DefaultGeneratorLoggerTests, SampleTestCaseNoOutputNeededFailure) {
    EXPECT_CALL(engine,
                logListItem1(2, "Problem does not need test case outputs, but this sample test case has output"));
    logger.logSampleTestCaseNoOutputNeededFailure();
}

TEST_F(DefaultGeneratorLoggerTests, ConstraintsVerificationFailure_WithSubtasks) {
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

TEST_F(DefaultGeneratorLoggerTests, ConstraintsVerificationFailure_WithConstraintsAndSubtasks) {
    {
        InSequence sequence;
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy constraints, on:"));
        EXPECT_CALL(engine, logListItem2(3, "X <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Does not satisfy subtask 2, on constraints:"));
        EXPECT_CALL(engine, logListItem2(3, "A <= 10"));
        EXPECT_CALL(engine, logListItem2(3, "B <= 10"));
        EXPECT_CALL(engine, logListItem1(2, "Satisfies subtask 1 but is not assigned to it"));
    }
    ConstraintsVerificationResult result({{Subtask::MAIN_ID, {"X <= 10"}}, {2, {"A <= 10", "B <= 10"}}}, {1});
    logger.logConstraintsVerificationFailure(result);
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

}
