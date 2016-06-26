#include "gmock/gmock.h"
#include "../mock.hpp"

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
    TestConfig testConfig = TestConfigBuilder()
            .setTestCasesDir("dir")
            .setSolutionCommand("python Sol.py")
            .build();

    TestGroupGenerator generator = TestGroupGenerator(&testCaseGenerator, &logger);

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
        EXPECT_CALL(testCaseGenerator, generate(tc1, testConfig));
        EXPECT_CALL(testCaseGenerator, generate(tc2, testConfig));
    }

    TestGroupGenerationResult result = generator.generate(testGroup, testConfig);
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_TRUE(result.isSuccessful());
}

TEST_F(TestGroupGeneratorTests, Generation_Failed_SomeTestCaseFailed) {
    ON_CALL(testCaseGenerator, generate(tc2, _))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(new SimpleFailure("failure"))));

    TestGroupGenerationResult expectedResult = TestGroupGenerationResult(nullptr, {
            TestCaseGenerationResult::successfulResult(),
            TestCaseGenerationResult::failedResult(new SimpleFailure("failure"))});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestGroupIntroduction(7));
        EXPECT_CALL(testCaseGenerator, generate(tc1, testConfig));
        EXPECT_CALL(testCaseGenerator, generate(tc2, testConfig));
    }

    TestGroupGenerationResult result = generator.generate(testGroup, testConfig);
    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_FALSE(result.isSuccessful());
}

}
