#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../verifier/MockVerifier.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "MockGeneratorLogger.hpp"
#include "MockTestCaseGenerator.hpp"
#include "tcframe/generator/TestGroupGenerator.hpp"

using std::ostringstream;

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class TestGroupGeneratorTests : public Test {
protected:
    Mock(TestCaseGenerator) testCaseGenerator;
    Mock(Verifier) verifier;
    Mock(OperatingSystem) os;
    Mock(GeneratorLogger) logger;

    TestCase tc1 = TestCaseBuilder()
            .setId("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setApplier([] {})
            .build();
    TestCase tc2 = TestCaseBuilder()
            .setId("foo_2")
            .setDescription("N = 100")
            .setSubtaskIds({1, 2, 3})
            .setApplier([] {})
            .build();
    TestGroup testGroup = TestGroup(7, {tc1, tc2});
    GeneratorConfig config = GeneratorConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();

    TestGroupGenerator generator = TestGroupGenerator(&testCaseGenerator, &verifier, &os, &logger);

    void SetUp() {
        ON_CALL(testCaseGenerator, generate(_, _))
                .WillByDefault(Return(TestCaseGenerationResult::successfulResult()));
    }
};

TEST_F(TestGroupGeneratorTests, Generation_Successful) {
    TestGroupGenerationResult expectedResult = TestGroupGenerationResult(nullptr, {
            TestCaseGenerationResult::successfulResult(),
            TestCaseGenerationResult::successfulResult()});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(7));
        EXPECT_CALL(testCaseGenerator, generate(tc1, config));
        EXPECT_CALL(testCaseGenerator, generate(tc2, config));
    }

    TestGroupGenerationResult result = generator.generate(testGroup, config);
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_TRUE(result.isSuccessful());
}

TEST_F(TestGroupGeneratorTests, Generation_Failed_SomeTestCaseFailed) {
    ON_CALL(testCaseGenerator, generate(tc2, _))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(new SimpleFailure("failure"))));

    TestGroupGenerationResult expectedResult(nullptr, {
            TestCaseGenerationResult::successfulResult(),
            TestCaseGenerationResult::failedResult(new SimpleFailure("failure"))});

    TestGroupGenerationResult result = generator.generate(testGroup, config);
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_FALSE(result.isSuccessful());
}

TEST_F(TestGroupGeneratorTests, Generation_MultipleTestCases_Successful) {
    int T;

    GeneratorConfig config = GeneratorConfigBuilder()
            .setMultipleTestCasesCount(&T)
            .setSlug("foo")
            .setTestCasesDir("dir")
            .build();

    ON_CALL(verifier, verifyMultipleTestCasesConstraints())
            .WillByDefault(Return(MultipleTestCasesConstraintsVerificationResult({})));

    Failure* expectedCombinationFailure = nullptr;
    TestGroupGenerationResult expectedResult(expectedCombinationFailure, {
            TestCaseGenerationResult::successfulResult(),
            TestCaseGenerationResult::successfulResult()});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(7));
        EXPECT_CALL(testCaseGenerator, generate(tc1, config));
        EXPECT_CALL(testCaseGenerator, generate(tc2, config));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_7"));
        EXPECT_CALL(verifier, verifyMultipleTestCasesConstraints());
        EXPECT_CALL(os, combineMultipleTestCases("dir/foo_7", 2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationResult(
                MultipleTestCasesCombinationResult(expectedCombinationFailure)));
    }

    TestGroupGenerationResult result = generator.generate(testGroup, config);
    EXPECT_THAT(T, Eq(2));
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_TRUE(result.isSuccessful());
}


TEST_F(TestGroupGeneratorTests, Generation_MultipleTestCases_Failed_Verification) {
    int T;

    GeneratorConfig config = GeneratorConfigBuilder()
            .setMultipleTestCasesCount(&T)
            .setSlug("foo")
            .build();

    ON_CALL(verifier, verifyMultipleTestCasesConstraints())
            .WillByDefault(Return(MultipleTestCasesConstraintsVerificationResult({"T <= 20"})));

    Failure* expectedCombinationFailure = new MultipleTestCasesConstraintsVerificationFailure(
            MultipleTestCasesConstraintsVerificationResult({"T <= 20"}));
    TestGroupGenerationResult expectedResult(expectedCombinationFailure, {
            TestCaseGenerationResult::successfulResult(),
            TestCaseGenerationResult::successfulResult()});

    EXPECT_CALL(logger, logMultipleTestCasesCombinationResult(
            MultipleTestCasesCombinationResult(expectedCombinationFailure)));

    TestGroupGenerationResult result = generator.generate(testGroup, config);
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_FALSE(result.isSuccessful());
}

}
