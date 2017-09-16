#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../client/MockSpecClient.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../../util/TestUtils.hpp"
#include "MockGeneratorLogger.hpp"
#include "MockTestCaseGenerator.hpp"
#include "tcframe/runner/generator/Generator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

namespace tcframe {

class GeneratorTests : public Test {
protected:

    MOCK(SpecClient) specClient;
    MOCK(TestCaseGenerator) testCaseGenerator;
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
            .setHasTcOutput(true)
            .build();

    Generator generator = {&specClient, &testCaseGenerator, &os, &logger};

    void SetUp() {
        ON_CALL(specClient, hasMultipleTestCases())
                .WillByDefault(Return(false));
        ON_CALL(specClient, getMultipleTestCasesOutputPrefix())
                .WillByDefault(Return(optional<string>()));
        ON_CALL(testCaseGenerator, generate(_, _))
                .WillByDefault(Return(true));
    }
};

TEST_F(GeneratorTests, Generation) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(simpleTestSuite));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(specClient, setSeed(42));
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGenerator, generate(stc1, options));
        EXPECT_CALL(testCaseGenerator, generate(stc2, options));

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(options));
}

TEST_F(GeneratorTests, Generation_MultipleTestGroups) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuite));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
    }
    EXPECT_TRUE(generator.generate(options));
}

TEST_F(GeneratorTests, Generation_Failed) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(simpleTestSuite));
    ON_CALL(testCaseGenerator, generate(stc1, _))
            .WillByDefault(Return(false));

    EXPECT_CALL(logger, logFailedResult());

    EXPECT_FALSE(generator.generate(options));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(simpleTestSuite));
    ON_CALL(specClient, hasMultipleTestCases())
            .WillByDefault(Return(true));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(testCaseGenerator, generate(stc1, options));
        EXPECT_CALL(testCaseGenerator, generate(stc2, options));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(specClient, validateMultipleTestCasesInput(2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationSuccessfulResult());

        EXPECT_CALL(logger, logSuccessfulResult());
    }
    EXPECT_TRUE(generator.generate(options));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_MultipleTestGroups) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(testSuite));
    ON_CALL(specClient, hasMultipleTestCases())
            .WillByDefault(Return(true));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(TestGroup::SAMPLE_ID));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_sample"));
        EXPECT_CALL(logger, logTestGroupIntroduction(1));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_1"));
        EXPECT_CALL(logger, logTestGroupIntroduction(2));
        EXPECT_CALL(logger, logMultipleTestCasesCombinationIntroduction("foo_2"));
    }
    EXPECT_TRUE(generator.generate(options));
}

TEST_F(GeneratorTests, Generation_MultipleTestCases_Failed_Verification) {
    ON_CALL(specClient, getTestSuite())
            .WillByDefault(Return(simpleTestSuite));
    ON_CALL(specClient, hasMultipleTestCases())
            .WillByDefault(Return(true));
    MultipleTestCasesConstraintsVerificationResult verificationResult({"T <= 20"});
    ON_CALL(specClient, validateMultipleTestCasesInput(_))
            .WillByDefault(Throw(verificationResult.asFormattedError()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logMultipleTestCasesCombinationFailedResult());
        EXPECT_CALL(logger, logFormattedError(verificationResult.asFormattedError()));

        EXPECT_CALL(logger, logFailedResult());
    }
    EXPECT_FALSE(generator.generate(options));
}

}
