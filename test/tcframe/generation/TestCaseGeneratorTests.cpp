#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "../variable/MockIOVariablesPrinter.hpp"
#include "../verification/MockConstraintSuiteVerifier.hpp"
#include "tcframe/generation/TestCaseGenerator.hpp"

using std::ostringstream;

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::IsNull;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Test;
using ::testing::WhenDynamicCastTo;

namespace tcframe {

bool applied;

class TestCaseGeneratorTests : public Test {
protected:
    Mock(ConstraintSuiteVerifier) constraintSuiteVerifier;
    Mock(IOVariablesPrinter) ioVariablesPrinter;
    Mock(OperatingSystem) os;

    function<void()> closure = [&](){applied = true;};
    TestCaseData data = TestCaseDataBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setConstraintGroupIds({1, 2})
            .build();
    GeneratorConfig config = GeneratorConfigBuilder()
            .setTestCasesDir("dir")
            .setSolutionCommand("python Sol.py")
            .build();
    ostream* out = new ostringstream();

    TestCaseGenerator generator = TestCaseGenerator(
            &constraintSuiteVerifier,
            &ioVariablesPrinter,
            &os);

    void SetUp() {
        applied = false;

        ON_CALL(constraintSuiteVerifier, verify(set<int>{1, 2}))
                .WillByDefault(Return(ConstraintSuiteVerificationResult::validResult()));
        ON_CALL(os, openForWriting("dir/foo_1.in"))
                .WillByDefault(Return(out));
    }
};

TEST_F(TestCaseGeneratorTests, SuccessfulGeneration) {
    {
        InSequence sequence;
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioVariablesPrinter, printInput(out));
        EXPECT_CALL(os, closeOpenedWritingStream(out));
        EXPECT_CALL(os, execute("python Sol.py", "dir/foo_1.in", "dir/foo_1.out", _));
    }

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_THAT(applied, Eq(true));
    EXPECT_THAT(result.isSuccessful(), Eq(true));
    EXPECT_THAT(result.failure(), IsNull());
}

TEST_F(TestCaseGeneratorTests, FailedGeneration_VerificationFailure) {
    ConstraintSuiteVerificationResult verificationResult({{1, {"1 <= N <= 10"}}}, {});
    ON_CALL(constraintSuiteVerifier, verify(set<int>{1, 2}))
            .WillByDefault(Return(verificationResult));

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_THAT(applied, Eq(true));
    EXPECT_THAT(result.isSuccessful(), Eq(false));
    EXPECT_THAT(result.failure(), WhenDynamicCastTo<VerificationFailure*>(
                    Pointee(VerificationFailure(verificationResult))));
}

}
