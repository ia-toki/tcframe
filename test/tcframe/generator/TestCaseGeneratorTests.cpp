#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../io_manipulator/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../verifier/MockVerifier.hpp"
#include "MockGeneratorLogger.hpp"
#include "tcframe/generator/TestCaseGenerator.hpp"

using std::ostringstream;

using ::testing::_;
using ::testing::AllOf;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Truly;

namespace tcframe {

class TestCaseGeneratorTests : public Test {
public:
    static int N;

protected:
    MOCK(Verifier) verifier;
    MOCK(IOManipulator) ioManipulator;
    MOCK(OperatingSystem) os;
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
    GeneratorConfig config = GeneratorConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();
    ostringstream* outForInput = new ostringstream();
    ostringstream* outForOutput = new ostringstream();
    ExecutionInfo executionInfo = ExecutionInfoBuilder().setExitCode(0).build();
    ExecutionResult executionResult = ExecutionResult(executionInfo, new istringstream(), new istringstream());

    TestCaseGenerator generator = TestCaseGenerator(&verifier, &ioManipulator, &os, &logger);

    void SetUp() {
        ON_CALL(verifier, verifyConstraints(_))
                .WillByDefault(Return(ConstraintsVerificationResult::validResult()));
        ON_CALL(os, openForWriting(EndsWith(".in")))
                .WillByDefault(Return(outForInput));
        ON_CALL(os, openForWriting(EndsWith(".out")))
                .WillByDefault(Return(outForOutput));
        ON_CALL(os, execute(_))
                .WillByDefault(Return(executionResult));
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

int TestCaseGeneratorTests::N;

TEST_F(TestCaseGeneratorTests, Generation_Successful_Sample) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(os, execute(AllOf(
                Property(&ExecutionRequest::command, "python Sol.py"),
                Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_sample_1.in")),
                Property(&ExecutionRequest::outputFilename, optional<string>("dir/foo_sample_1.out")))));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, config));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful_Sample_WithOutput) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.out"));
        EXPECT_CALL(os, closeOpenedWritingStream(outForOutput));
        EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCaseWithOutput, config));
    EXPECT_THAT(outForInput->str(), Eq("42\n"));
    EXPECT_THAT(outForOutput->str(), Eq("yes\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful_Official) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(outForInput));
        EXPECT_CALL(os, closeOpenedWritingStream(outForInput));
        EXPECT_CALL(os, execute(AllOf(
                Property(&ExecutionRequest::command, "python Sol.py"),
                Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_1.in")),
                Property(&ExecutionRequest::outputFilename, optional<string>("dir/foo_1.out")))));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(officialTestCase, config));
    EXPECT_THAT(N, Eq(42));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputApplication_Sample) {
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

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputApplication_Official) {
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

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputGeneration) {
    string message = "output error";
    ON_CALL(ioManipulator, parseOutput(executionResult.outputStream()))
            .WillByDefault(Throw(runtime_error(message)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseFailedResult(optional<string>("N = 42")));
        EXPECT_CALL(logger, logSimpleFailure(message));
    }
    EXPECT_FALSE(generator.generate(officialTestCase, config));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputApplication_Sample) {
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

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputApplication_Official) {
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
