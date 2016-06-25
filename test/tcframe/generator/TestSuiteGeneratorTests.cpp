#include "gmock/gmock.h"
#include "../helper.hpp"
#include "../mock.hpp"

#include <vector>

#include "../io/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "MockTestGroupGenerator.hpp"
#include "MockGeneratorLogger.hpp"
#include "tcframe/generator/TestSuiteGenerator.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Ne;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;
using ::testing::Unused;
using ::testing::WithArg;

using std::vector;

namespace tcframe {

int N;

class TestSuiteGeneratorTests : public Test {
protected:
    Mock(TestGroupGenerator) testGroupGenerator;
    Mock(IOManipulator) ioManipulator;
    Mock(OperatingSystem) os;
    Mock(GeneratorLogger) logger;

    vector<string> stc1 = {"10", "20"};
    vector<string> stc2 = {"30"};

    OfficialTestCase tc1 = OfficialTestCase([&] {N = 1;}, "N = 1");
    OfficialTestCase tc2 = OfficialTestCase([&] {N = 2;}, "N = 2");
    OfficialTestCase tc3 = OfficialTestCase([&] {N = 3;}, "N = 3");

    TestSuite testSuite = TestSuiteBuilder()
            .addSampleTestCase(stc1)
            .addSampleTestCase(stc2)
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .addOfficialTestCase(tc3)
            .build();
    TestSuite testSuiteWithGroups = TestSuiteBuilder()
            .addSampleTestCase(stc1, {1, 2})
            .addSampleTestCase(stc2, {2})
            .newTestGroup()
            .setSubtaskIds({1, 2})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .setSubtaskIds({2})
            .addOfficialTestCase(tc3)
            .build();

    ProblemConfig problemConfig = ProblemConfigBuilder()
            .setSlug("foo")
            .build();
    TestConfig testConfig = TestConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();
    CoreConfig coreConfig = CoreConfig(problemConfig, testConfig);

    TestSuiteGenerator generator = TestSuiteGenerator(&testGroupGenerator, &ioManipulator, &os, &logger);

    static TestGroupGenerationResult createSuccessfulTestGroupResult(TestGroup testGroup, Unused) {
        vector<TestCaseGenerationResult> testCaseResults;
        for (int i = 0; i < testGroup.testCases().size(); i++) {
            testCaseResults.push_back(TestCaseGenerationResult::successfulResult());
        }
        return TestGroupGenerationResult(nullptr, testCaseResults);
    }

    void SetUp() {
        ON_CALL(testGroupGenerator, generate(_, _))
                .WillByDefault(Invoke(&createSuccessfulTestGroupResult));
    }
};

TEST_F(TestSuiteGeneratorTests, Generation_Successful) {
    GenerationResult expectedResult({
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()})});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(0, {
                TestCaseBuilder().setName("foo_sample_1").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_sample_2").setSubtaskIds({-1}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(-1, {
                TestCaseBuilder().setName("foo_1").setDescription("N = 1").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_2").setDescription("N = 2").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_3").setDescription("N = 3").setSubtaskIds({-1}).build()}),
                testConfig));
    }
    GenerationResult result = generator.generate(testSuite, coreConfig);

    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_TRUE(result.isSuccessful());
}

TEST_F(TestSuiteGeneratorTests, Generation_Failed) {
    ON_CALL(testGroupGenerator, generate(Property(&TestGroup::id, 0), _))
            .WillByDefault(Return(TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed"))})));

    GenerationResult expectedResult({
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed"))}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()})});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(0, {
                TestCaseBuilder().setName("foo_sample_1").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_sample_2").setSubtaskIds({-1}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(-1, {
                TestCaseBuilder().setName("foo_1").setDescription("N = 1").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_2").setDescription("N = 2").setSubtaskIds({-1}).build(),
                TestCaseBuilder().setName("foo_3").setDescription("N = 3").setSubtaskIds({-1}).build()}),
                testConfig));
    }
    GenerationResult result = generator.generate(testSuite, coreConfig);

    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_FALSE(result.isSuccessful());
}


TEST_F(TestSuiteGeneratorTests, Generation_WithGroups_Successful) {
    GenerationResult expectedResult({
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult()})});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(0, {
                TestCaseBuilder().setName("foo_sample_1").setSubtaskIds({1, 2}).build(),
                TestCaseBuilder().setName("foo_sample_2").setSubtaskIds({2}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(1, {
                TestCaseBuilder().setName("foo_1_1").setDescription("N = 1").setSubtaskIds({1, 2}).build(),
                TestCaseBuilder().setName("foo_1_2").setDescription("N = 2").setSubtaskIds({1, 2}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(2, {
                TestCaseBuilder().setName("foo_2_1").setDescription("N = 3").setSubtaskIds({2}).build()}),
                testConfig));
    }
    GenerationResult result = generator.generate(testSuiteWithGroups, coreConfig);

    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_TRUE(result.isSuccessful());
}


TEST_F(TestSuiteGeneratorTests, Generation_WithGroups_Failed) {
    ON_CALL(testGroupGenerator, generate(Property(&TestGroup::id, 1), _))
            .WillByDefault(Return(TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed 1")),
                    TestCaseGenerationResult::successfulResult()})));
    ON_CALL(testGroupGenerator, generate(Property(&TestGroup::id, 2), _))
            .WillByDefault(Return(TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed 2"))})));

    GenerationResult expectedResult({
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::successfulResult(),
                    TestCaseGenerationResult::successfulResult()}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed 1")),
                    TestCaseGenerationResult::successfulResult()}),
            TestGroupGenerationResult(nullptr, {
                    TestCaseGenerationResult::failedResult(new SimpleFailure("failed 2"))})});
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(0, {
                TestCaseBuilder().setName("foo_sample_1").setSubtaskIds({1, 2}).build(),
                TestCaseBuilder().setName("foo_sample_2").setSubtaskIds({2}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(1, {
                TestCaseBuilder().setName("foo_1_1").setDescription("N = 1").setSubtaskIds({1, 2}).build(),
                TestCaseBuilder().setName("foo_1_2").setDescription("N = 2").setSubtaskIds({1, 2}).build()}),
                testConfig));

        EXPECT_CALL(testGroupGenerator, generate(TestGroup(2, {
                TestCaseBuilder().setName("foo_2_1").setDescription("N = 3").setSubtaskIds({2}).build()}),
                testConfig));
    }
    GenerationResult result = generator.generate(testSuiteWithGroups, coreConfig);

    EXPECT_THAT(result, Eq(expectedResult));
    EXPECT_FALSE(result.isSuccessful());
}


TEST_F(TestSuiteGeneratorTests, Generation_InputFinalizer_Official_Applied) {
    TestSuite testSuite = TestSuiteBuilder()
            .setInputFinalizer([] {N *= 2;})
            .addOfficialTestCase(tc3)
            .build();

    Captor<TestGroup> testGroupCaptor;
    ON_CALL(testGroupGenerator, generate(_, _))
            .WillByDefault(DoAll(
                    WithArg<0>(Invoke(&testGroupCaptor, &Captor<TestGroup>::capture)),
                    InvokeWithoutArgs([] {return TestGroupGenerationResult(
                            nullptr, {TestCaseGenerationResult::successfulResult()});})));

    generator.generate(testSuite, coreConfig);

    testGroupCaptor.arg().testCases()[0].applier()();
    EXPECT_THAT(N, Eq(3 * 2));
}

TEST_F(TestSuiteGeneratorTests, Generation_InputFinalizer_Sample_NotApplied) {
    TestSuite testSuite = TestSuiteBuilder()
            .setInputFinalizer([] {N *= 2;})
            .addSampleTestCase(stc1)
            .build();

    ON_CALL(ioManipulator, parseInput(_))
            .WillByDefault(InvokeWithoutArgs([] {N = 10;}));

    Captor<TestGroup> testGroupCaptor;
    ON_CALL(testGroupGenerator, generate(_, _))
            .WillByDefault(DoAll(
                    WithArg<0>(Invoke(&testGroupCaptor, &Captor<TestGroup>::capture)),
                    InvokeWithoutArgs([]{return TestGroupGenerationResult(
                            nullptr, {TestCaseGenerationResult::successfulResult()});})));

    generator.generate(testSuite, coreConfig);

    testGroupCaptor.arg().testCases()[0].applier()();
    EXPECT_THAT(N, Ne(10 * 2));
    EXPECT_THAT(N, Eq(10));
}

}
