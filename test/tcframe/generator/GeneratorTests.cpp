#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../io_manipulator/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
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
    MOCK(TestCaseGenerator) testCaseGenerator;
    MOCK(Verifier) verifier;
    MOCK(OperatingSystem) os;
    MOCK(GeneratorLogger) logger;

    TestCase stc1 = TestUtils::createFakeTestCase("foo_sample_1");
    TestCase stc2 = TestUtils::createFakeTestCase("foo_sample_2");
    TestCase tc1 = TestUtils::createFakeTestCase("foo_1_1");
    TestCase tc2 = TestUtils::createFakeTestCase("foo_1_2");
    TestCase tc3 = TestUtils::createFakeTestCase("foo_2_1");

    TestSuite simpleTestSuite = TestSuite({
            TestGroup(0, {stc1, stc2})});

    TestSuite testSuite = TestSuite({
            TestGroup(0, {stc1, stc2}),
            TestGroup(1, {tc1, tc2}),
            TestGroup(2, {tc3})});

    GeneratorConfig config = GeneratorConfigBuilder()
            .setSlug("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();

    GeneratorConfig multipleTestCasesConfig = GeneratorConfigBuilder(config)
            .setMultipleTestCasesCount(&T)
            .build();

    Generator generator = Generator(&testCaseGenerator, &verifier, &os, &logger);

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
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseGenerator, generate(stc1, config));
        EXPECT_CALL(testCaseGenerator, generate(stc2, config));

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(simpleTestSuite, config));
}

TEST_F(GeneratorTests, Generation_Successful_MultipleTestGroups) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
    }
    EXPECT_TRUE(generator.generate(testSuite, config));
}

TEST_F(GeneratorTests, Generation_Failed) {
    ON_CALL(testCaseGenerator, generate(stc1, _))
            .WillByDefault(Return(false));

    EXPECT_CALL(logger, logFailedResult());

    EXPECT_FALSE(generator.generate(simpleTestSuite, config));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(testCaseGenerator, generate(stc1, multipleTestCasesConfig));
        EXPECT_CALL(testCaseGenerator, generate(stc2, multipleTestCasesConfig));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(verifier, verifyMultipleTestCasesConstraints());
        EXPECT_CALL(os, combineMultipleTestCases("dir/foo_sample", 2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationSuccessfulResult());

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(simpleTestSuite, multipleTestCasesConfig));
    EXPECT_THAT(T, Eq(2));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Successful_MultipleTestGroups) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(0));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_1"));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_2"));
    }
    EXPECT_TRUE(generator.generate(testSuite, multipleTestCasesConfig));
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
    EXPECT_FALSE(generator.generate(simpleTestSuite, multipleTestCasesConfig));
}

}
