#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../io_manipulator/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../spec/core/MockSeedSetter.hpp"
#include "../util/TestUtils.hpp"
#include "../verifier/MockVerifier.hpp"
#include "MockGeneratorLogger.hpp"
#include "MockTestCaseGenerator.hpp"
#include "tcframe/generator/Generator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

int T;

class GeneratorTests : public Test {
protected:
    MOCK(SeedSetter) seedSetter;
    MOCK(TestCaseGenerator) testCaseGenerator;
    MOCK(Verifier) verifier;
    MOCK(OperatingSystem) os;
    MOCK(GeneratorLogger) logger;

    TestCase stc1 = TestUtils::newTestCase("foo_sample_1");
    TestCase stc2 = TestUtils::newTestCase("foo_sample_2");
    TestCase tc1 = TestUtils::newTestCase("foo_1_1");
    TestCase tc2 = TestUtils::newTestCase("foo_1_2");
    TestCase tc3 = TestUtils::newTestCase("foo_2_1");

    TestSuite simpleTestSuite = TestSuite({
            TestGroup(TestGroup::SAMPLE_ID, {stc1, stc2})});

    TestSuite testSuite = TestSuite({
            TestGroup(TestGroup::SAMPLE_ID, {stc1, stc2}),
            TestGroup(1, {tc1, tc2}),
            TestGroup(2, {tc3})});

    GenerationOptions options = GenerationOptionsBuilder("foo")
            .setSeed(42)
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .setNeedsOutput(true)
            .build();

    GenerationOptions multipleTestCasesOptions = GenerationOptionsBuilder(options)
            .setMultipleTestCasesCounter(&T)
            .build();

    GenerationOptions multipleTestCasesOptionsWithOutputPrefix = GenerationOptionsBuilder(multipleTestCasesOptions)
            .setMultipleTestCasesCounter(&T)
            .setMultipleTestCasesOutputPrefix("Case #%d: ")
            .build();

    Generator generator = Generator(&seedSetter, &testCaseGenerator, &verifier, &os, &logger);

    void SetUp() {
        ON_CALL(testCaseGenerator, generate(_, _))
                .WillByDefault(Return(true));
        ON_CALL(verifier, verifyMultipleTestCasesConstraints())
                .WillByDefault(Return(MultipleTestCasesConstraintsVerificationResult({})));
    }
};

TEST_F(GeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(seedSetter, setSeed(42));
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGenerator, generate(stc1, options));
        EXPECT_CALL(testCaseGenerator, generate(stc2, options));

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(simpleTestSuite, options));
}

TEST_F(GeneratorTests, Generation_Successful_MultipleTestGroups) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
    }
    EXPECT_TRUE(generator.generate(testSuite, options));
}

TEST_F(GeneratorTests, Generation_Failed) {
    ON_CALL(testCaseGenerator, generate(stc1, _))
            .WillByDefault(Return(false));

    EXPECT_CALL(logger, logFailedResult());

    EXPECT_FALSE(generator.generate(simpleTestSuite, options));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGenerator, generate(stc1, multipleTestCasesOptions));
        EXPECT_CALL(testCaseGenerator, generate(stc2, multipleTestCasesOptions));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(verifier, verifyMultipleTestCasesConstraints());
        EXPECT_CALL(logger, logMultipleTestCasesCombinationSuccessfulResult());

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(simpleTestSuite, multipleTestCasesOptions));
    EXPECT_THAT(T, Eq(2));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Successful_MultipleTestGroups) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_1"));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_2"));
    }
    EXPECT_TRUE(generator.generate(testSuite, multipleTestCasesOptions));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Failed_Verification) {
    MultipleTestCasesConstraintsVerificationResult verificationResult({"T <= 20"});
    ON_CALL(verifier, verifyMultipleTestCasesConstraints())
            .WillByDefault(Return(verificationResult));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logMultipleTestCasesCombinationFailedResult());
        EXPECT_CALL(logger, logMultipleTestCasesConstraintsVerificationFailure(verificationResult));

        EXPECT_CALL(logger, logFailedResult());
    }
    EXPECT_FALSE(generator.generate(simpleTestSuite, multipleTestCasesOptions));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_WithOutputPrefix_Successful) {
    EXPECT_CALL(logger, logSuccessfulResult());
    EXPECT_TRUE(generator.generate(simpleTestSuite, multipleTestCasesOptionsWithOutputPrefix));
    EXPECT_THAT(T, Eq(2));
}

}
