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

class BaseTestSpecTests : public Test {
protected:
    class ProblemSpec : public BaseProblemSpec {
    public: // intentionaly public so that input values can be inspected
        int A, B;

        void InputFormat() {}
    };

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        void InputFinalizer() {
            A *= 2;
        }
    };

    class TestSpecWithTestCases : public TestSpec {
    protected:
        void SampleTestCase1() {
            Input({"10", "20"});
            Output({"yes"});
        }

        void SampleTestCase2() {
            Input({"30", "40"});
        }

        void TestCases() {
            addOfficialTestCase([=] {A = 1, B = 2;}, "A = 1, B = 2");
            addOfficialTestCase([=] {A = 3, B = 4;}, "A = 3, B = 4");
        }
    };

    class TestSpecWithTestGroups : public TestSpec {
    protected:
        void SampleTestCase1() {
            Subtasks({1, 2});
            Input({
                "1",
                "2"
            });
            Output({
                "yes"
            });
        }

        void SampleTestCase2() {
            Subtasks({2});
            Input({
                "123",
                "456"
            });
        }

        void TestGroup1() {
            Subtasks({1, 2});

            addOfficialTestCase([=] {A = 1, B = 2;}, "A = 1, B = 2");
            addOfficialTestCase([=] {A = 3, B = 4;}, "A = 3, B = 4");
            addOfficialTestCase([=] {A = 5, B = 6;}, "A = 5, B = 6");
        }

        void TestGroup2() {
            Subtasks({2});

            addOfficialTestCase([=] {A = 101, B = 201;}, "A = 101, B = 201");
            addOfficialTestCase([=] {A = 301, B = 401;}, "A = 301, B = 401");
        }
    };

    // Testing that rnd is available in TestSpec
    class TestSpecWithRandom : public TestSpec {
    protected:
        void TestCases() {
            addOfficialTestCase([=] {A = rnd.nextInt(1, 100);}, "A = rnd.nextInt(1, 100)");
        }
    };

    TestSpecWithTestCases problemSpecWithTestCases;
    TestSpecWithTestGroups problemSpecWithTestGroups;
};

TEST_F(BaseTestSpecTests, InputFinalizer) {
    TestSuite testSuite = problemSpecWithTestCases.buildTestSuite("foo");
    OfficialTestCaseData* data = (OfficialTestCaseData*) testSuite.testGroups()[1].testCases()[1].data();

    data->closure()();
    EXPECT_THAT(problemSpecWithTestCases.A, Eq(3 * 2));
}

TEST_F(BaseTestSpecTests, TestSuite) {
    TestSuite testSuite = problemSpecWithTestCases.buildTestSuite("foo");
    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            AllOf(Property(&TestGroup::id, 0), Property(&TestGroup::testCases, SizeIs(2))),
            AllOf(Property(&TestGroup::id, -1), Property(&TestGroup::testCases, SizeIs(2)))));
}

TEST_F(BaseTestSpecTests, TestSuite_WithGroups) {
    TestSuite testSuite = problemSpecWithTestGroups.buildTestSuite("foo");
    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            AllOf(Property(&TestGroup::id, 0), Property(&TestGroup::testCases, SizeIs(2))),
            AllOf(Property(&TestGroup::id, 1), Property(&TestGroup::testCases, SizeIs(3))),
            AllOf(Property(&TestGroup::id, 2), Property(&TestGroup::testCases, SizeIs(2)))));
}

}
