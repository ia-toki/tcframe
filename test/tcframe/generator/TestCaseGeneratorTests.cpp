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
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Test;
using ::testing::WhenDynamicCastTo;

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
    GeneratorConfig config = GeneratorConfigBuilder()
            .setTestCasesDir("dir")
            .setSolutionCommand("python Sol.py")
            .build();
    ostream* out = new ostringstream();

    TestCaseGenerator generator = TestCaseGenerator(
            &verifier,
            &ioManipulator,
            &os);

    void SetUp() {
        applied = false;

        ON_CALL(verifier, verify(set<int>{1, 2}))
                .WillByDefault(Return(VerificationResult::validResult()));
        ON_CALL(os, openForWriting("dir/foo_1.in"))
                .WillByDefault(Return(out));
    }
};

TEST_F(TestCaseGeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(os, openForWriting("dir/foo_1.in"));
        EXPECT_CALL(ioManipulator, printInput(out));
        EXPECT_CALL(os, closeOpenedWritingStream(out));
        EXPECT_CALL(os, execute("python Sol.py", "dir/foo_1.in", "dir/foo_1.out", _));
    }

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_TRUE(applied);
    EXPECT_TRUE(result.isSuccessful());
    EXPECT_THAT(result.failure(), IsNull());
}

TEST_F(TestCaseGeneratorTests, Generation_Failed_VerificationFailure) {
    VerificationResult verificationResult({{1, {"1 <= N <= 10"}}}, {});
    ON_CALL(verifier, verify(set<int>{1, 2}))
            .WillByDefault(Return(verificationResult));

    TestCaseGenerationResult result = generator.generate(data, closure, config);

    EXPECT_TRUE(applied);
    EXPECT_FALSE(result.isSuccessful());
    EXPECT_THAT(result.failure(), WhenDynamicCastTo<VerificationFailure*>(
            Pointee(VerificationFailure(verificationResult))));
}

}
