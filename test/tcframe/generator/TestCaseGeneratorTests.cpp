#include "gmock/gmock.h"
#include "../helper.hpp"
#include "../mock.hpp"

#include <sstream>

#include "../evaluator/MockEvaluator.hpp"
#include "../io_manipulator/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../scorer/MockScorer.hpp"
#include "../verifier/MockVerifier.hpp"
#include "MockGeneratorLogger.hpp"
#include "tcframe/generator/TestCaseGenerator.hpp"

using ::testing::_;
using ::testing::AllOf;
using ::testing::DoAll;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Property;
using ::testing::Return;
using ::testing::StartsWith;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Truly;
using ::testing::WithArg;

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
    MOCK(Scorer) scorer;
    MOCK(GeneratorLogger) logger;

    TestCase sampleTestCase = TestCaseBuilder()
            .setId("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n"))
            .build();
    TestCase sampleTestCaseWithOutput = TestCaseBuilder()
            .setId("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n", "yes\n"))
            .build();
    TestCase officialTestCase = TestCaseBuilder()
            .setId("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{N = 42;}))
            .build();
    TestCase officialTestCaseWithInvalidClosure = TestCaseBuilder()
            .setId("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{throw runtime_error("closure error");}))
            .build();
    GeneratorConfig config = GeneratorConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();
    GeneratorConfig multipleTestCasesConfig = GeneratorConfigBuilder(config)
            .setMultipleTestCasesCounter(&T)
            .build();
    GeneratorConfig multipleTestCasesConfigWithOutputPrefix = GeneratorConfigBuilder(multipleTestCasesConfig)
            .setMultipleTestCasesCounter(&T)
            .setMultipleTestCasesOutputPrefix("Case #%d: ")
            .build();
    EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .build();

    ostringstream* outForInput = new ostringstream();
    ostringstream* outForOutput = new ostringstream();
    ostringstream* outForEvaluation = new ostringstream();
    ExecutionInfo executionInfo = ExecutionInfoBuilder().setExitCode(0).build();
    ExecutionResult executionResult = ExecutionResultBuilder()
            .setInfo(executionInfo)
            .setOutputStream(new istringstream("yes\n"))
            .build();
    ExecutionResult executionResultWithOutputPrefix = ExecutionResultBuilder()
            .setInfo(executionInfo)
            .setOutputStream(new istringstream("Case #1: yes\n"))
            .build();

    TestCaseGenerator generator = TestCaseGenerator(&verifier, &ioManipulator, &os, &evaluator, &scorer, &logger);

    Captor<string> evaluationCaptor1;
    Captor<string> evaluationCaptor2;

    void SetUp() {
        ON_CALL(verifier, verifyConstraints(_))
                .WillByDefault(Return(ConstraintsVerificationResult::validResult()));
        ON_CALL(os, openForWriting(EndsWith(".in")))
                .WillByDefault(Return(outForInput));
        ON_CALL(os, openForWriting(EndsWith(".out")))
                .WillByDefault(Return(outForOutput));
        ON_CALL(os, openForWriting(StartsWith("_")))
                .WillByDefault(DoAll(
                        WithArg<0>(Invoke(&evaluationCaptor1, &Captor<string>::capture)),
                        InvokeWithoutArgs([&] {return outForEvaluation;})));
        ON_CALL(os, execute(_))
                .WillByDefault(Return(executionResult));
        ON_CALL(evaluator, evaluate(_, _, _))
                .WillByDefault(Return(EvaluationResultBuilder()
                        .setExecutionResult(executionResult)
                        .build()));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(DoAll(
                        WithArg<2>(Invoke(&evaluationCaptor2, &Captor<string>::capture)),
                        InvokeWithoutArgs([] {return ScoringResultBuilder().setVerdict(Verdict::ac()).build();})));
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
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(evaluator, evaluate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluatorConfig));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, config));
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
    EXPECT_FALSE(generator.generate(sampleTestCase, config));
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
    EXPECT_FALSE(generator.generate(sampleTestCase, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(evaluator, evaluate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluatorConfig));
        EXPECT_CALL(os, openForWriting(_));
        EXPECT_CALL(os, closeOpenedWritingStream(outForEvaluation));
        EXPECT_CALL(scorer, score("dir/foo_sample_1.in", "dir/foo_sample_1.out", _));
        EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, config));
    EXPECT_THAT(evaluationCaptor1.arg(), Eq(evaluationCaptor2.arg()));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
    EXPECT_THAT(outForEvaluation->str(), Eq("yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_Check) {
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(ScoringResultBuilder()
                    .setVerdict(Verdict::wa())
                    .setMessage("diff")
                    .build()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseCheckFailure("diff"));
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(outForInput));
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(evaluator, evaluate("dir/foo_1.in", "dir/foo_1.out", evaluatorConfig));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(officialTestCase, config));
    EXPECT_THAT(N, Eq(42));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_Successful) {
    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesConfig));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Sample_WithOutput_Successful) {
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResultBuilder()
                    .setExecutionResult(executionResultWithOutputPrefix)
                    .build()));

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, multipleTestCasesConfigWithOutputPrefix));
    EXPECT_THAT(outForEvaluation->str(), Eq("Case #1: yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Successful) {
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResultBuilder()
                    .setExecutionResult(executionResultWithOutputPrefix)
                    .build()));

    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesConfigWithOutputPrefix));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Failed) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure("Output must start with \"Case #%d: \""));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, multipleTestCasesConfigWithOutputPrefix));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputApplication) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure("closure error"));
    }
    EXPECT_FALSE(generator.generate(officialTestCaseWithInvalidClosure, config));
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
    EXPECT_FALSE(generator.generate(officialTestCase, config));
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
    EXPECT_FALSE(generator.generate(officialTestCase, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputEvaluation) {
    ExecutionResult executionResult = ExecutionResultBuilder()
            .setInfo(ExecutionInfoBuilder().setExitCode(1).build())
            .build();
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(EvaluationResultBuilder()
                    .setExecutionResult(executionResult)
                    .build()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSolutionExecutionFailure(executionResult));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, config));
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
    EXPECT_FALSE(generator.generate(officialTestCase, config));
}

}
