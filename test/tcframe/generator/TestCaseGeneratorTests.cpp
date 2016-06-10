#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../io/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../verifier/MockConstraintSuiteVerifier.hpp"
#include "tcframe/generator/TestCaseGenerator.hpp"

using std::ostringstream;

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::IsNull;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

namespace tcframe {

bool applied;

class TestCaseGeneratorTests : public Test {
protected:
    Mock(ConstraintSuiteVerifier) verifier;
    Mock(IOManipulator) ioManipulator;
    Mock(OperatingSystem) os;

    function<void()> closure = [&](){applied = true;};
    TestCaseData data = TestCaseDataBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setConstraintGroupIds({1, 2})
            .build();
    TestConfig config = TestConfigBuilder()
            .setTestCasesDir("dir")
            .setSolutionCommand("python Sol.py")
            .build();
    ostream* out = new ostringstream();
    ExecutionResult executionResult = ExecutionResult(0, new istringstream(), new istringstream());

    TestCaseGenerator generator = TestCaseGenerator(
            &verifier,
            &ioManipulator,
            &os);

    void SetUp() {
        applied = false;

        ON_CALL(verifier, verify(_))
                .WillByDefault(Return(VerificationResult::validResult()));
        ON_CALL(os, openForWriting(_))
                .WillByDefault(Return(out));
        ON_CALL(os, execute(_, _, _, _))
                .WillByDefault(Return(executionResult));
    }
};

TEST_F(TestCaseGeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(verifier, verify(set<int>{1, 2}));
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(out));
        EXPECT_CALL(os, closeOpenedWritingStream(out));
        EXPECT_CALL(os, execute("python Sol.py", "dir/foo_1.in", "dir/foo_1.out", _));
        EXPECT_CALL(ioManipulator, parseOutput(executionResult.outputStream()));
    }

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_TRUE(applied);
    EXPECT_TRUE(result.isSuccessful());
    EXPECT_THAT(result.failure(), IsNull());
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_Verification) {
    VerificationResult verificationResult({{1, {"1 <= N <= 10"}}}, {});
    ON_CALL(verifier, verify(set<int>{1, 2}))
            .WillByDefault(Return(verificationResult));

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_TRUE(result.failure()->equals(new VerificationFailure(verificationResult)));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_InputGeneration) {
    ON_CALL(ioManipulator, printInput(out))
            .WillByDefault(Throw(runtime_error("input error")));

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_TRUE(result.failure()->equals(new OtherFailure("input error")));
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_OutputGeneration) {
    ON_CALL(ioManipulator, parseOutput(executionResult.outputStream()))
            .WillByDefault(Throw(runtime_error("output error")));

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_TRUE(result.failure()->equals(new OtherFailure("output error")));
}

}
