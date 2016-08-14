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
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

namespace tcframe {

class TestCaseGeneratorTests : public Test {
protected:
    Mock(Verifier) verifier;
    Mock(IOManipulator) ioManipulator;
    Mock(OperatingSystem) os;
    Mock(GeneratorLogger) logger;

    static int N;

    TestCase sampleTestCase = TestCaseBuilder()
            .setId("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n"))
            .build();
    TestCase officialTestCase = TestCaseBuilder()
            .setId("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{N = 42;}))
            .build();
    GeneratorConfig config = GeneratorConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();
    ostringstream* out = new ostringstream();
    ExecutionInfo executionInfo = ExecutionInfoBuilder().setExitCode(0).build();
    ExecutionResult executionResult = ExecutionResult(executionInfo, new istringstream(), new istringstream());

    TestCaseGenerator generator = TestCaseGenerator(&verifier, &ioManipulator, &os, &logger);

    void SetUp() {
        ON_CALL(verifier, verifyConstraints(_))
                .WillByDefault(Return(ConstraintsVerificationResult::validResult()));
        ON_CALL(os, openForWriting(_))
                .WillByDefault(Return(out));
        ON_CALL(os, execute(_))
                .WillByDefault(Return(executionResult));
    }
};

int TestCaseGeneratorTests::N;

TEST_F(TestCaseGeneratorTests, Generation_Successful_Sample) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_sample_1.in"));
        EXPECT_CALL(os, closeOpenedWritingStream(out));
        EXPECT_CALL(os, execute(AllOf(
                Property(&ExecutionRequest::command, "python Sol.py"),
                Property(&ExecutionRequest::inputFilename, optional<string>("dir/foo_sample_1.in")),
                Property(&ExecutionRequest::outputFilename, optional<string>("dir/foo_sample_1.out")))));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
        EXPECT_CALL(logger, logTestCaseSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(sampleTestCase, config));
    EXPECT_THAT(out->str(), Eq("42\n"));
}

TEST_F(TestCaseGeneratorTests, Generation_Successful_Official) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(out));
        EXPECT_CALL(os, closeOpenedWritingStream(out));
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
    ON_CALL(ioManipulator, printInput(out))
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

}
