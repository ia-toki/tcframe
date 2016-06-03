#include "gmock/gmock.h"
#include "../mock.hpp"

#include <vector>

#include "../io/MockIOManipulator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "FakeTestCaseGenerationFailure.hpp"
#include "MockTestCaseGenerator.hpp"
#include "MockGenerationLogger.hpp"
#include "tcframe/generator/Generator.hpp"

using ::testing::_;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Pair;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;

using std::vector;

namespace tcframe {

class GeneratorTests : public Test {
protected:
    Mock(TestCaseGenerator) testCaseGenerator;
    Mock(IOManipulator) ioManipulator;
    Mock(OperatingSystem) os;
    Mock(GenerationLogger) logger;

    vector<string> stc1 = {"10", "20"};
    vector<string> stc2 = {"30"};

    OfficialTestCase tc1 = OfficialTestCase([]{}, "N = 1");
    OfficialTestCase tc2 = OfficialTestCase([]{}, "N = 2");
    OfficialTestCase tc3 = OfficialTestCase([]{}, "N = 3");

    TestSuite testSuiteWithoutGroups = TestSuiteBuilder()
            .addSampleTestCase(stc1)
            .addSampleTestCase(stc2)
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .build();
    TestSuite testSuiteWithGroups = TestSuiteBuilder()
            .addSampleTestCase(stc1, {1, 2})
            .addSampleTestCase(stc2, {2})
            .newTestGroup()
            .setConstraintGroupIds({1, 2})
            .addOfficialTestCase(tc1)
            .addOfficialTestCase(tc2)
            .newTestGroup()
            .setConstraintGroupIds({2})
            .addOfficialTestCase(tc3)
            .build();

    ProblemConfig problemConfig = ProblemConfigBuilder()
            .setSlug("foo")
            .build();
    GeneratorConfig generatorConfig = GeneratorConfigBuilder()
            .setSolutionCommand("python Sol.py")
            .setTestCasesDir("dir")
            .build();

    Generator generator = Generator(&testCaseGenerator, &ioManipulator, &os, &logger);

    void SetUp() {
        ON_CALL(testCaseGenerator, generate(_, _, generatorConfig))
                .WillByDefault(Return(TestCaseGenerationResult::successfulResult()));
    }
};

TEST_F(GeneratorTests, Generation_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logSampleTestCasesIntroduction());

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_1").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_2").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));


        EXPECT_CALL(logger, logTestGroupIntroduction(-1));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1").setDescription("N = 1").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 1", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_2").setDescription("N = 2").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 2", TestCaseGenerationResult::successfulResult()));
    }

    GenerationResult result = generator.generate(testSuiteWithoutGroups, problemConfig, generatorConfig);

    EXPECT_TRUE(result.isSuccessful());
    EXPECT_THAT(result.resultsByName(), ElementsAre(
            Pair("foo_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_2", TestCaseGenerationResult::successfulResult()),
            Pair("foo_sample_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_sample_2", TestCaseGenerationResult::successfulResult())));
}

TEST_F(GeneratorTests, Generation_Failed) {
    TestCaseGenerationFailure* failure_sample_1 = new FakeTestCaseGenerationFailure();
    ON_CALL(testCaseGenerator, generate(Property(&TestCaseData::name, "foo_sample_1"), _, generatorConfig))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(failure_sample_1)));
    TestCaseGenerationFailure* failure_2 = new FakeTestCaseGenerationFailure();
    ON_CALL(testCaseGenerator, generate(Property(&TestCaseData::name, "foo_2"), _, generatorConfig))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(failure_2)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logSampleTestCasesIntroduction());

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_1").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::failedResult(failure_sample_1)));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_2").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));


        EXPECT_CALL(logger, logTestGroupIntroduction(-1));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1").setDescription("N = 1").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 1", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_2").setDescription("N = 2").setConstraintGroupIds({-1}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 2", TestCaseGenerationResult::failedResult(failure_2)));
    }

    GenerationResult result = generator.generate(testSuiteWithoutGroups, problemConfig, generatorConfig);

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_THAT(result.resultsByName(), ElementsAre(
            Pair("foo_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_2", TestCaseGenerationResult::failedResult(failure_2)),
            Pair("foo_sample_1", TestCaseGenerationResult::failedResult(failure_sample_1)),
            Pair("foo_sample_2", TestCaseGenerationResult::successfulResult())));
}

TEST_F(GeneratorTests, Generation_WithGroups_Successful) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logSampleTestCasesIntroduction());

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_1").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_2").setConstraintGroupIds({2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));


        EXPECT_CALL(logger, logTestGroupIntroduction(1));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1_1").setDescription("N = 1").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 1", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1_2").setDescription("N = 2").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 2", TestCaseGenerationResult::successfulResult()));


        EXPECT_CALL(logger, logTestGroupIntroduction(2));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_2_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_2_1").setDescription("N = 3").setConstraintGroupIds({2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 3", TestCaseGenerationResult::successfulResult()));
    }

    GenerationResult result = generator.generate(testSuiteWithGroups, problemConfig, generatorConfig);

    EXPECT_TRUE(result.isSuccessful());
    EXPECT_THAT(result.resultsByName(), ElementsAre(
            Pair("foo_1_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_1_2", TestCaseGenerationResult::successfulResult()),
            Pair("foo_2_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_sample_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_sample_2", TestCaseGenerationResult::successfulResult())));
}

TEST_F(GeneratorTests, Generation_WithGroups_Failed) {
    TestCaseGenerationFailure* failure_sample_1 = new FakeTestCaseGenerationFailure();
    ON_CALL(testCaseGenerator, generate(Property(&TestCaseData::name, "foo_sample_1"), _, generatorConfig))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(failure_sample_1)));
    TestCaseGenerationFailure* failure_1_2 = new FakeTestCaseGenerationFailure();
    ON_CALL(testCaseGenerator, generate(Property(&TestCaseData::name, "foo_1_2"), _, generatorConfig))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(failure_1_2)));
    TestCaseGenerationFailure* failure_2_1 = new FakeTestCaseGenerationFailure();
    ON_CALL(testCaseGenerator, generate(Property(&TestCaseData::name, "foo_2_1"), _, generatorConfig))
            .WillByDefault(Return(TestCaseGenerationResult::failedResult(failure_2_1)));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logIntroduction());
        EXPECT_CALL(os, forceMakeDir("dir"));

        EXPECT_CALL(logger, logSampleTestCasesIntroduction());

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_1").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::failedResult(failure_sample_1)));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_sample_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_sample_2").setConstraintGroupIds({2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("", TestCaseGenerationResult::successfulResult()));


        EXPECT_CALL(logger, logTestGroupIntroduction(1));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1_1").setDescription("N = 1").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 1", TestCaseGenerationResult::successfulResult()));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1_2"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_1_2").setDescription("N = 2").setConstraintGroupIds({1, 2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 2", TestCaseGenerationResult::failedResult(failure_1_2)));


        EXPECT_CALL(logger, logTestGroupIntroduction(2));

        EXPECT_CALL(logger, logTestCaseIntroduction("foo_2_1"));
        EXPECT_CALL(testCaseGenerator, generate(
                TestCaseDataBuilder().setName("foo_2_1").setDescription("N = 3").setConstraintGroupIds({2}).build(),
                _,
                generatorConfig));
        EXPECT_CALL(logger, logTestCaseGenerationResult("N = 3", TestCaseGenerationResult::failedResult(failure_2_1)));
    }

    GenerationResult result = generator.generate(testSuiteWithGroups, problemConfig, generatorConfig);

    EXPECT_FALSE(result.isSuccessful());
    EXPECT_THAT(result.resultsByName(), ElementsAre(
            Pair("foo_1_1", TestCaseGenerationResult::successfulResult()),
            Pair("foo_1_2", TestCaseGenerationResult::failedResult(failure_1_2)),
            Pair("foo_2_1", TestCaseGenerationResult::failedResult(failure_2_1)),
            Pair("foo_sample_1", TestCaseGenerationResult::failedResult(failure_sample_1)),
            Pair("foo_sample_2", TestCaseGenerationResult::successfulResult())));
}

}
