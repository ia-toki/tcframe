#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../evaluator/MockEvaluator.hpp"
#include "../io_manipulator/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../verifier/MockVerifier.hpp"
#include "MockGeneratorLogger.hpp"
#include "tcframe/generator/TestCaseGenerator.hpp"

using ::testing::_;
using ::testing::AllOf;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::StartsWith;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Truly;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class TestCaseGeneratorTests : public Test {
public:
    static int T;
    static int N;

protected:
    MOCK(Verifier) verifier;
    MOCK(IOManipulator) ioManipulator;
    MOCK(OperatingSystem) os;
    MOCK(Evaluator) evaluator;
    MOCK(GeneratorLogger) logger;

    TestCase sampleTestCase = TestCaseBuilder()
            .setName("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n"))
            .build();
    TestCase sampleTestCaseWithOutput = TestCaseBuilder()
            .setName("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n", "yes\n"))
            .build();
    TestCase officialTestCase = TestCaseBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{N = 42;}))
            .build();
    TestCase officialTestCaseWithInvalidClosure = TestCaseBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{throw runtime_error("closure error");}))
            .build();
    GenerationOptions options = GenerationOptionsBuilder("foo")
            .setSeed(0)
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setNeedsOutput(true)
            .build();
    GenerationOptions noOutputOptions = GenerationOptionsBuilder(options)
            .setNeedsOutput(false)
            .build();
    GenerationOptions multipleTestCasesOptions = GenerationOptionsBuilder(options)
            .setMultipleTestCasesCounter(&T)
            .build();
    GenerationOptions multipleTestCasesOptionsWithOutputPrefix = GenerationOptionsBuilder(multipleTestCasesOptions)
            .setMultipleTestCasesCounter(&T)
            .setMultipleTestCasesOutputPrefix("Case #%d: ")
            .build();
    GenerationOptions multipleTestCasesNoOutputOptions = GenerationOptionsBuilder(multipleTestCasesOptions)
            .setNeedsOutput(false)
            .build();
    EvaluationOptions evaluationOptions = EvaluationOptionsBuilder()
            .setSolutionCommand("python Sol.py")
            .build();

    istringstream* inForOutput = new istringstream();
    ostringstream* outForInput = new ostringstream();
    ostringstream* outForOutput = new ostringstream();
    ostringstream* outForEvaluation = new ostringstream();

    TestCaseGenerator generator = TestCaseGenerator(&verifier, &ioManipulator, &os, &evaluator, &logger);

    void SetUp() {
        ON_CALL(verifier, verifyConstraints(_))
                .WillByDefault(Return(ConstraintsVerificationResult::validResult()));
        ON_CALL(os, openForReading(EndsWith(".out")))
                .WillByDefault(Return(inForOutput));
        ON_CALL(os, openForWriting(EndsWith(".in")))
                .WillByDefault(Return(outForInput));
        ON_CALL(os, openForWriting(EndsWith(".out")))
                .WillByDefault(Return(outForOutput));
        ON_CALL(os, openForWriting(StartsWith("_")))
                .WillByDefault(Return(outForEvaluation));
        ON_CALL(os, execute(_))
                .WillByDefault(Return(ExecutionResult()));
        ON_CALL(evaluator, generate(_, _, _))
                .WillByDefault(Return(GenerationResult(optional<Verdict>(), ExecutionResult())));
        ON_CALL(evaluator, score(_, _))
                .WillByDefault(Return(ScoringResult(Verdict(), ExecutionResult())));
    }

    struct InputStreamContentIs {
        string content_;

        InputStreamContentIs(const string& content)
                : content_(content) {}

        bool operator()(istream* in) const {
            istringstream* sin = (istringstream*) in;
            return sin->str() == content_;
        }
    };
};

int TestCaseGeneratorTests::T;
int TestCaseGeneratorTests::N;

TEST_F(TestCaseGeneratorTests, Generation_Sample_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(evaluator, generate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluationOptions));
        EXPECT_CALL(os, openForReading("dir/foo_sample_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, options));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_NoOutput_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_CALL(evaluator, evaluate(_, _, _)).Times(0);
    EXPECT_CALL(ioManipulator, parseOutput(_)).Times(0);
    EXPECT_TRUE(generator.generate(sampleTestCase, noOutputOptions));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_Failed_InputApplication) {
    string message = "input error";
    ON_CALL(ioManipulator, parseInput(_))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSimpleFailure(message));
    }
    EXPECT_FALSE(generator.generate(sampleTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_Failed_OutputApplication) {
    string message = "output error";
    ON_CALL(ioManipulator, parseOutput(_))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSimpleFailure(message));
    }
    EXPECT_FALSE(generator.generate(sampleTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(evaluator, generate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluationOptions));
        EXPECT_CALL(os, openForWriting(Evaluator::EVALUATION_OUT_FILENAME));
        EXPECT_CALL(os, closeOpenedStream(outForEvaluation));
        EXPECT_CALL(evaluator, score("dir/foo_sample_1.in", "dir/foo_sample_1.out"));
        EXPECT_CALL(os, openForReading("dir/foo_sample_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, options));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
    EXPECT_THAT(outForEvaluation->str(), Eq("yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_Check) {
    Verdict verdict = Verdict(VerdictStatus::wa());
    ExecutionResult scoringExecutionResult = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("diff")
            .build();
    ON_CALL(evaluator, score(_, _))
            .WillByDefault(Return(ScoringResult(verdict, scoringExecutionResult)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseCheckFailure());
        EXPECT_CALL(logger, logExecutionResults(map<string, ExecutionResult>{
                {"scorer", scoringExecutionResult}}));
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_NoOutputNeeded) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseNoOutputNeededFailure());
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, noOutputOptions));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(outForInput));
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(evaluator, generate("dir/foo_1.in", "dir/foo_1.out", evaluationOptions));
        EXPECT_CALL(os, openForReading("dir/foo_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(officialTestCase, options));
    EXPECT_THAT(N, Eq(42));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_Successful) {
    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesOptions));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_NoOutput_Successful) {
    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesNoOutputOptions));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Sample_WithOutput_Successful) {
    ON_CALL(os, openForReading(EndsWith(".out")))
            .WillByDefault(Return(new istringstream("Case #1: yes\n")));

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, multipleTestCasesOptionsWithOutputPrefix));
    EXPECT_THAT(outForEvaluation->str(), Eq("Case #1: yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Successful) {
    ON_CALL(os, openForReading(EndsWith(".out")))
            .WillByDefault(Return(new istringstream("Case #1: yes\n")));

    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesOptionsWithOutputPrefix));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure("Output must start with \"Case #%d: \""));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, multipleTestCasesOptionsWithOutputPrefix));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputApplication) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure("closure error"));
    }
    EXPECT_FALSE(generator.generate(officialTestCaseWithInvalidClosure, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_Verification) {
    ConstraintsVerificationResult verificationResult({{1, {"1 <= N <= 10"}}}, {});
    ON_CALL(verifier, verifyConstraints(set<int>{1, 2}))
            .WillByDefault(Return(verificationResult));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logConstraintsVerificationFailure(verificationResult));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputGeneration) {
    string message = "input error";
    ON_CALL(ioManipulator, printInput(outForInput))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure(message));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputGeneration) {
    Verdict verdict(VerdictStatus::rte());
    ExecutionResult executionResult = ExecutionResultBuilder().setExitCode(1).build();
    ON_CALL(evaluator, generate(_, _, _))
            .WillByDefault(Return(GenerationResult(optional<Verdict>(verdict), executionResult)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logExecutionResults(map<string, ExecutionResult>{{"solution", executionResult}}));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputApplication) {
    string message = "output error";
    ON_CALL(ioManipulator, parseOutput(_))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure(message));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, options));
}

}
