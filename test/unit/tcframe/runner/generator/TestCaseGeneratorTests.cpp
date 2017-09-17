#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>
#include <utility>

#include "../client/MockSpecClient.hpp"
#include "../evaluator/MockEvaluator.hpp"
#include "MockGeneratorLogger.hpp"
#include "tcframe/runner/generator/TestCaseGenerator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Truly;
using ::testing::WhenDynamicCastTo;

using std::istringstream;
using std::move;
using std::ostringstream;

namespace tcframe {

class TestCaseGeneratorTests : public Test {
public:
    static int N;

protected:
    MOCK(SpecClient) specClient;
    MOCK(Evaluator) evaluator;
    MOCK(GeneratorLogger) logger;

    TestCase sampleTestCase = TestCaseBuilder()
            .setName("foo_sample_1")
            .setData(new SampleTestCaseData("42\n", "yes\n"))
            .build();
    TestCase sampleTestCaseWithoutOutput = TestCaseBuilder()
            .setName("foo_sample_1")
            .setData(new SampleTestCaseData("42\n"))
            .build();
    TestCase officialTestCase = TestCaseBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setData(new OfficialTestCaseData([&]{N = 42;}))
            .build();
    GenerationOptions options = GenerationOptionsBuilder("foo")
            .setSeed(0)
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setHasTcOutput(true)
            .build();
    GenerationOptions noOutputOptions = GenerationOptionsBuilder(options)
            .setHasTcOutput(false)
            .build();
    EvaluationOptions evaluationOptions = EvaluationOptionsBuilder()
            .setSolutionCommand("python Sol.py")
            .build();

    TestCaseGenerator generator = {&specClient, &evaluator, &logger};

    void SetUp() {
        ON_CALL(evaluator, generate(_, _, _))
                .WillByDefault(Return(GenerationResult(optional<Verdict>(), ExecutionResult())));
        ON_CALL(evaluator, score(_, _))
                .WillByDefault(Return(ScoringResult(Verdict(), ExecutionResult())));
    }

    struct SimpleErrorMessageIs {
        string message_;

        explicit SimpleErrorMessageIs(string message)
                : message_(move(message)) {}

        bool operator()(runtime_error* e) const {
            return message_ == e->what();
        }
    };
};

int TestCaseGeneratorTests::N;

TEST_F(TestCaseGeneratorTests, Generation_Sample) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(specClient, generateTestCaseInput("foo_sample_1", "dir/foo_sample_1.in"));
        EXPECT_CALL(evaluator, generate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluationOptions));
        EXPECT_CALL(specClient, generateSampleTestCaseOutput("foo_sample_1", Evaluator::EVALUATION_OUT_FILENAME));
        EXPECT_CALL(evaluator, score("dir/foo_sample_1.in", "dir/foo_sample_1.out"));
        EXPECT_CALL(specClient, validateTestCaseOutput("dir/foo_sample_1.out"));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_Failed_Check) {
    Verdict verdict(VerdictStatus::wa());
    auto scoringExecutionResult = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("diff")
            .build();
    ON_CALL(evaluator, score(_, _))
            .WillByDefault(Return(ScoringResult(verdict, scoringExecutionResult)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));

        FormattedError error = FormattedError::combine({
                FormattedError(
                        {{0, "Sample test case output does not match with actual output produced by the solution"}}),
                ExecutionResults::asFormattedError({{"scorer", scoringExecutionResult}})});
        EXPECT_CALL(logger, logError(WhenDynamicCastTo<FormattedError*>(Pointee(error))));
    }
    EXPECT_FALSE(generator.generate(sampleTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_Failed_SampleOutputGeneration) {
    string message = "error";
    ON_CALL(specClient, generateSampleTestCaseOutput(_, _))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logError(Truly(SimpleErrorMessageIs(message))));
    }
    EXPECT_FALSE(generator.generate(sampleTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_NoOutputNeeded) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logError(Truly(SimpleErrorMessageIs(
                "Problem does not need test case outputs, but this sample test case has output"))));
    }
    EXPECT_FALSE(generator.generate(sampleTestCase, noOutputOptions));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithoutOutput) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(specClient, generateTestCaseInput("foo_sample_1", "dir/foo_sample_1.in"));
        EXPECT_CALL(evaluator, generate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluationOptions));
        EXPECT_CALL(specClient, validateTestCaseOutput("dir/foo_sample_1.out"));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_CALL(specClient, generateSampleTestCaseOutput(_, _)).Times(0);
    EXPECT_CALL(evaluator, score(_, _)).Times(0);
    EXPECT_TRUE(generator.generate(sampleTestCaseWithoutOutput, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Official) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(specClient, generateTestCaseInput("foo_1", "dir/foo_1.in"));
        EXPECT_CALL(evaluator, generate("dir/foo_1.in", "dir/foo_1.out", evaluationOptions));
        EXPECT_CALL(specClient, validateTestCaseOutput("dir/foo_1.out"));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Official_NoOutput) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(specClient, generateTestCaseInput("foo_1", "dir/foo_1.in"));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_CALL(evaluator, evaluate(_, _, _)).Times(0);
    EXPECT_CALL(specClient, validateTestCaseOutput(_)).Times(0);
    EXPECT_TRUE(generator.generate(officialTestCase, noOutputOptions));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputGeneration) {
    string message = "input error";
    ON_CALL(specClient, generateTestCaseInput(_, _))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logError(Truly(SimpleErrorMessageIs(message))));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputGeneration) {
    Verdict verdict(VerdictStatus::rte());
    auto executionResult = ExecutionResultBuilder().setExitCode(1).build();
    ON_CALL(evaluator, generate(_, _, _))
            .WillByDefault(Return(GenerationResult(optional<Verdict>(verdict), executionResult)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logError(
                WhenDynamicCastTo<FormattedError*>(Pointee(
                        ExecutionResults::asFormattedError({{"solution", executionResult}})))));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputValidation) {
    string message = "output error";
    ON_CALL(specClient, validateTestCaseOutput(_))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logError(Truly(SimpleErrorMessageIs(message))));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

}
