#include "gmock/gmock.h"

#include "tcframe/spec/core/BaseProblemSpec.hpp"
#include "tcframe/spec/core/BaseTestSpec.hpp"

using ::testing::A;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Property;
using ::testing::SizeIs;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class BaseTestSpecTests : public Test {
protected:
    class ProblemSpec : public BaseProblemSpec {
    public: // intentionaly public so that input values can be inspected
        int A, B;
        vector<int> V;

    protected:
        void InputFormat() {}
    };

    class ProblemSpecWithSubtasks : public ProblemSpec {
    protected:
        void Subtask1() {}
        void Subtask2() {}
    };

    class TestSpecWithLifecycle : public BaseTestSpec<ProblemSpec> {
    protected:
        void BeforeTestCase() {
            V.clear();
        }

        void AfterTestCase() {
            V.push_back(0);
        }

        void TestCases() {
            addOfficialTestCase([=] {V.push_back(1);}, "V.push_back(1)");
            addOfficialTestCase([=] {V.push_back(2);}, "V.push_back(2)");
        }
    };

    class TestSpecWithTestCases : public BaseTestSpec<ProblemSpec> {
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

    class TestSpecWithTestGroups : public BaseTestSpec<ProblemSpecWithSubtasks> {
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

    class TestSpecWithInvalidTestGroups : public BaseTestSpec<ProblemSpecWithSubtasks> {
    protected:
        void TestGroup1() {
            Subtasks({1, 3, 5});
        }
    };

    // Testing that rnd is available in TestSpec
    class TestSpecWithRandom : public BaseTestSpec<ProblemSpec> {
    protected:
        void TestCases() {
            addOfficialTestCase([=] {A = rnd.nextInt(1, 100);}, "A = rnd.nextInt(1, 100)");
        }
    };

    TestSpecWithLifecycle testSpecWithLifecycle;
    TestSpecWithTestCases testSpecWithTestCases;
    TestSpecWithTestGroups testSpecWithTestGroups;
    TestSpecWithInvalidTestGroups testSpecWithInvalidTestGroups;
};

TEST_F(BaseTestSpecTests, Lifecycle) {
    TestSuite testSuite = testSpecWithLifecycle.buildTestSuite("foo", {});
    auto tc1 = (OfficialTestCaseData*) testSuite.testGroups()[1].testCases()[0].data();
    auto tc2 = (OfficialTestCaseData*) testSuite.testGroups()[1].testCases()[1].data();

    tc1->closure()();
    EXPECT_THAT(testSpecWithLifecycle.V, Eq(vector<int>{1, 0}));

    tc2->closure()();
    EXPECT_THAT(testSpecWithLifecycle.V, Eq(vector<int>{2, 0}));
}

TEST_F(BaseTestSpecTests, TestSuite) {
    TestSuite testSuite = testSpecWithTestCases.buildTestSuite("foo", {});
    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            AllOf(Property(&TestGroup::id, TestGroup::SAMPLE_ID), Property(&TestGroup::testCases, SizeIs(2))),
            AllOf(Property(&TestGroup::id, TestGroup::MAIN_ID), Property(&TestGroup::testCases, SizeIs(2)))));
}

TEST_F(BaseTestSpecTests, TestSuite_WithGroups) {
    TestSuite testSuite = testSpecWithTestGroups.buildTestSuite("foo", {1, 2});
    EXPECT_THAT(testSuite.testGroups(), ElementsAre(
            AllOf(Property(&TestGroup::id, TestGroup::SAMPLE_ID), Property(&TestGroup::testCases, SizeIs(2))),
            AllOf(Property(&TestGroup::id, 1), Property(&TestGroup::testCases, SizeIs(3))),
            AllOf(Property(&TestGroup::id, 2), Property(&TestGroup::testCases, SizeIs(2)))));
}

TEST_F(BaseTestSpecTests, TestSuite_Invalid_UndefinedSubtaskIds) {
    try {
        testSpecWithInvalidTestGroups.buildTestSuite("foo", {1, 2});
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The following subtasks are referenced but not defined: {3, 5}"));
    }
}

}
