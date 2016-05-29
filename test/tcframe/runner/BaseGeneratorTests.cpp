#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using ::testing::A;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Property;
using ::testing::SizeIs;
using ::testing::Test;

namespace tcframe {

class BaseGeneratorTests : public Test {
protected:
    class FakeProblem : public BaseProblem {
    protected:
        int A, B;

        void InputFormat() {}
    };

    class FakeGenerator : public BaseGenerator<FakeProblem> {
    protected:
        void Config() {
            setSolutionCommand("python Sol.py");
            setTestCasesDir("dir");
        }
    };

    class GeneratorWithoutTestGroups : public FakeGenerator {
    protected:
        void SampleTestCases() {
            addSampleTestCase({"10", "20"});
            addSampleTestCase({"30", "40"});
        }

        void TestCases() {
            addOfficialTestCase(OfficialTestCase([=] {A = 1, B = 2;}, "A = 1, B = 2"));
            addOfficialTestCase(OfficialTestCase([=] {A = 3, B = 4;}, "A = 3, B = 4"));
        }
    };

    class GeneratorWithTestGroups : public FakeGenerator {
    protected:
        void SampleTestCases() {
            addSampleTestCase({"10", "20"}, {1, 2});
            addSampleTestCase({"30", "40"}, {2});
        }

        void TestGroup1() {
            assignToSubtasks({1, 2});

            addOfficialTestCase(OfficialTestCase([=] {A = 1, B = 2;}, "A = 1, B = 2"));
            addOfficialTestCase(OfficialTestCase([=] {A = 3, B = 4;}, "A = 3, B = 4"));
            addOfficialTestCase(OfficialTestCase([=] {A = 5, B = 6;}, "A = 5, B = 6"));
        }

        void TestGroup2() {
            assignToSubtasks({2});

            addOfficialTestCase(OfficialTestCase([=] {A = 101, B = 201;}, "A = 101, B = 201"));
            addOfficialTestCase(OfficialTestCase([=] {A = 301, B = 401;}, "A = 301, B = 401"));
        }
    };

    FakeGenerator generator;
    GeneratorWithoutTestGroups generatorWithoutTestGroups;
    GeneratorWithTestGroups generatorWithTestGroups;
};

TEST_F(BaseGeneratorTests, Config) {
    GeneratorConfig config = generator.buildGeneratorConfig();
    EXPECT_THAT(config.solutionCommand(), Eq("python Sol.py"));
    EXPECT_THAT(config.testCasesDir(), Eq("dir"));
}

TEST_F(BaseGeneratorTests, TestSuite) {
    TestSuite testSuite = generatorWithoutTestGroups.buildTestSuite();
    EXPECT_THAT(testSuite.sampleTests(), SizeIs(2));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&TestGroup::id, -1), Property(&TestGroup::officialTestCases, SizeIs(2)))));
}

TEST_F(BaseGeneratorTests, TestSuite_WithGroups) {
    TestSuite testSuite = generatorWithTestGroups.buildTestSuite();
    EXPECT_THAT(testSuite.sampleTests(), SizeIs(2));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&TestGroup::id, 1), Property(&TestGroup::officialTestCases, SizeIs(3))),
            AllOf(Property(&TestGroup::id, 2), Property(&TestGroup::officialTestCases, SizeIs(2)))));
}

}
