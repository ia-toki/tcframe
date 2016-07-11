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
        void SampleTestCases() {
            addSampleTestCase({"10", "20"});
            addSampleTestCase({"30", "40"});
        }

        void TestCases() {
            addOfficialTestCase(OfficialTestCase([=] {A = 1, B = 2;}, "A = 1, B = 2"));
            addOfficialTestCase(OfficialTestCase([=] {A = 3, B = 4;}, "A = 3, B = 4"));
        }
    };

    class TestSpecWithTestGroups : public TestSpec {
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

    // Testing that rnd is available in TestSpec
    class TestSpecWithRandom : public TestSpec {
    protected:
        void TestCases() {
            addOfficialTestCase(OfficialTestCase([=] {A = rnd.nextInt(1, 100);}, "A = rnd.nextInt(1, 100)"));
        }
    };

    TestSpec spec;
    TestSpecWithTestCases specWithTestCases;
    TestSpecWithTestGroups specWithTestGroups;
};

TEST_F(BaseTestSpecTests, InputFinalizer) {
    TestSuite testSuite = spec.buildTestSuite();
    function<void()> inputFinalizer = testSuite.inputFinalizer();

    spec.A = 3;
    inputFinalizer();

    EXPECT_THAT(spec.A, Eq(3 * 2));
}

TEST_F(BaseTestSpecTests, TestSuite) {
    TestSuite testSuite = specWithTestCases.buildTestSuite();
    EXPECT_THAT(testSuite.sampleTests(), SizeIs(2));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&OfficialTestGroup::id, -1), Property(&OfficialTestGroup::officialTestCases, SizeIs(2)))));
}

TEST_F(BaseTestSpecTests, TestSuite_WithGroups) {
    TestSuite testSuite = specWithTestGroups.buildTestSuite();
    EXPECT_THAT(testSuite.sampleTests(), SizeIs(2));
    EXPECT_THAT(testSuite.officialTests(), ElementsAre(
            AllOf(Property(&OfficialTestGroup::id, 1), Property(&OfficialTestGroup::officialTestCases, SizeIs(3))),
            AllOf(Property(&OfficialTestGroup::id, 2), Property(&OfficialTestGroup::officialTestCases, SizeIs(2)))));
}

}
