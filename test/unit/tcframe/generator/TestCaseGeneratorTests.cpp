#include "gmock/gmock.h"
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
    GeneratorConfig noOutputConfig = GeneratorConfigBuilder(config)
            .setNeedsOutput(false)
            .build();
    GeneratorConfig multipleTestCasesConfig = GeneratorConfigBuilder(config)
            .setMultipleTestCasesCounter(&T)
            .build();
    GeneratorConfig multipleTestCasesConfigWithOutputPrefix = GeneratorConfigBuilder(multipleTestCasesConfig)
            .setMultipleTestCasesCounter(&T)
            .setMultipleTestCasesOutputPrefix("Case #%d: ")
            .build();
    GeneratorConfig multipleTestCasesNoOutputConfig = GeneratorConfigBuilder(multipleTestCasesConfig)
            .setNeedsOutput(false)
            .build();
    EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .build();

    istringstream* inForOutput = new istringstream();
    ostringstream* outForInput = new ostringstream();
    ostringstream* outForOutput = new ostringstream();
    ostringstream* outForEvaluation = new ostringstream();

    TestCaseGenerator generator = TestCaseGenerator(&verifier, &ioManipulator, &os, &evaluator, &scorer, &logger);

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
        ON_CALL(evaluator, evaluate(_, _, _))
                .WillByDefault(Return(EvaluationResult()));
        ON_CALL(scorer, score(_, _, _))
                .WillByDefault(Return(ScoringResultBuilder()
                        .setExecutionResult(ExecutionResult())
                        .setVerdict(Verdict::ac())
                        .build()));
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
        EXPECT_CALL(evaluator, evaluate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluatorConfig));
        EXPECT_CALL(os, openForReading("dir/foo_sample_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, config));
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
    EXPECT_TRUE(generator.generate(sampleTestCase, noOutputConfig));
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
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(evaluator, evaluate("dir/foo_sample_1.in", "dir/foo_sample_1.out", evaluatorConfig));
        EXPECT_CALL(os, openForWriting(Evaluator::EVALUATION_FILENAME));
        EXPECT_CALL(os, closeOpenedStream(outForEvaluation));
        EXPECT_CALL(scorer, score("dir/foo_sample_1.in", "dir/foo_sample_1.out", Evaluator::EVALUATION_FILENAME));
        EXPECT_CALL(os, openForReading("dir/foo_sample_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, config));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
    EXPECT_THAT(outForEvaluation->str(), Eq("yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_Check_WA) {
    ScoringResult scoringResult = ScoringResultBuilder()
            .setExecutionResult(ExecutionResult())
            .setVerdict(Verdict::wa())
            .setPrivateInfo("diff")
            .build();
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(scoringResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setScoringResult(scoringResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseCheckFailure());
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_Check_ERR) {
    ScoringResult scoringResult = ScoringResultBuilder()
            .setExecutionResult(ExecutionResultBuilder()
                    .setExitCode(1)
                    .setStandardError("error")
                    .build())
            .setVerdict(Verdict::err())
            .build();
    ON_CALL(scorer, score(_, _, _))
            .WillByDefault(Return(scoringResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setScoringResult(scoringResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseCheckFailure());
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Sample_WithOutput_Failed_NoOutputNeeded) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>()));
        EXPECT_CALL(logger, logSampleTestCaseNoOutputNeededFailure());
    }

    EXPECT_FALSE(generator.generate(sampleTestCaseWithOutput, noOutputConfig));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(outForInput));
        EXPECT_CALL(os, closeOpenedStream(outForInput));
        EXPECT_CALL(evaluator, evaluate("dir/foo_1.in", "dir/foo_1.out", evaluatorConfig));
        EXPECT_CALL(os, openForReading("dir/foo_1.out"));
        EXPECT_CALL(ioManipulator, parseOutput(inForOutput));
        EXPECT_CALL(os, closeOpenedStream(inForOutput));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(officialTestCase, config));
    EXPECT_THAT(N, Eq(42));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_Successful) {
    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesConfig));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_NoOutput_Successful) {
    EXPECT_TRUE(generator.generate(officialTestCase, multipleTestCasesNoOutputConfig));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Sample_WithOutput_Successful) {
    ON_CALL(os, openForReading(EndsWith(".out")))
            .WillByDefault(Return(new istringstream("Case #1: yes\n")));

    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, multipleTestCasesConfigWithOutputPrefix));
    EXPECT_THAT(outForEvaluation->str(), Eq("Case #1: yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Successful) {
    ON_CALL(os, openForReading(EndsWith(".out")))
            .WillByDefault(Return(new istringstream("Case #1: yes\n")));

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
    EvaluationResult evaluationResult = EvaluationResultBuilder()
            .setExecutionResult(ExecutionResultBuilder().setExitCode(1).build())
            .build();
    ON_CALL(evaluator, evaluate(_, _, _))
            .WillByDefault(Return(evaluationResult));

    TestCaseGrade grade = TestCaseGradeCreator()
            .setEvaluationResult(evaluationResult)
            .create();
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logTestCaseGradeDetails(grade));
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
