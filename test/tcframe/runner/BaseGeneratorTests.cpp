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
    void TestCases() {
        CASE(A = 1, B = 2);
        CASE(A = 3, B = 4);
    }
};

class GeneratorWithTestGroups : public FakeGenerator {
protected:
    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 2);
        CASE(A = 3, B = 4);
        CASE(A = 5, B = 6);
    }


    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 101, B = 201);
        CASE(A = 301, B = 401);
    }
};

class BaseGeneratorTests : public Test {
protected:
    FakeGenerator generator;
    GeneratorWithoutTestGroups generatorWithoutTestGroups;
    GeneratorWithTestGroups generatorWithTestGroups;
};

TEST_F(BaseGeneratorTests, Config) {
    GeneratorConfig config = generator.buildGeneratorConfig();
    EXPECT_THAT(config.solutionCommand(), Eq("python Sol.py"));
    EXPECT_THAT(config.testCasesDir(), Eq("dir"));
}

TEST_F(BaseGeneratorTests, TestCases) {
    TestSuite testSuite = generatorWithoutTestGroups.buildTestSuite();
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&TestGroup::id, -1), Property(&TestGroup::officialTestCases, SizeIs(2)))));
}

TEST_F(BaseGeneratorTests, TestGroups) {
    TestSuite testSuite = generatorWithTestGroups.buildTestSuite();
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&TestGroup::id, 1), Property(&TestGroup::officialTestCases, SizeIs(3))),
            AllOf(Property(&TestGroup::id, 2), Property(&TestGroup::officialTestCases, SizeIs(2)))));
}

}
