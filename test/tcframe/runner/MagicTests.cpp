#include "gmock/gmock.h"

#include "../variable/FakeVariable.hpp"
#include "tcframe/experimental/runner.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::SizeIs;
using ::testing::Test;

namespace tcframe {

class MagicTests : public Test {
protected:
    class CONS_Tester : public ConstraintSuiteBuilder {
    protected:
        int A, B;

    public:
        void testValid() {
            CONS(1 <= A && A <= 100);
            CONS(1 <= B && B <= 100);
        }
    };

    class CASE_Tester : public TestSuiteBuilder {
    protected:
        int A, B;

    public:
        void testValid() {
            CASE(A = 1, B = 2);
            CASE(A = 3, B = 4);
        }
    };

    class SAMPLE_CASE_Tester : public TestSuiteBuilder {
    public:
        void testValidWithoutGroups() {
            SAMPLE_CASE({
                "10 20",
                "30"
            });
            SAMPLE_CASE({
                "40 50",
                "60"
            });
        }

        void testValidWithGroups() {
            SAMPLE_CASE({
                "10 20",
                "30"
            }, {1, 2});
            SAMPLE_CASE({
                "40 50",
                "60"
            }, {2});
        }
    };

    class LINE_Tester : public IOFormatBuilder {
    protected:
        int A, B;
        vector<int> C, D;

    public:
        void testValid() {
            LINE(A);
            LINE(A, B);
            LINE(A, B, C % SIZE(3), D);
        }
    };
};

TEST_F(MagicTests, VariableNamesExtractor) {
    VariableNamesExtractor extractor("A, B, C % SIZE(3), D");
    EXPECT_THAT(extractor.nextName(), Eq("A"));
    EXPECT_THAT(extractor.nextName(), Eq("B"));
    EXPECT_THAT(extractor.nextName(), Eq("C"));
    EXPECT_THAT(extractor.nextName(), Eq("D"));
}

TEST_F(MagicTests, CONS_Valid) {
    CONS_Tester tester;
    tester.testValid();
    ConstraintSuite constraintSuite = tester.build();

    vector<Constraint> constraints = constraintSuite.individualConstraints()[0].constraints();

    ASSERT_THAT(constraints, SizeIs(2));
    EXPECT_THAT(constraints[0].description(), Eq("1 <= A && A <= 100"));
    EXPECT_THAT(constraints[1].description(), Eq("1 <= B && B <= 100"));
}

TEST_F(MagicTests, CASE_Valid) {
    CASE_Tester tester;
    tester.testValid();
    TestSuite testSuite = tester.build();

    vector<OfficialTestCase> officialTestCases = testSuite.officialTests()[0].officialTestCases();

    ASSERT_THAT(officialTestCases, SizeIs(2));
    EXPECT_THAT(officialTestCases[0].description(), Eq("A = 1, B = 2"));
    EXPECT_THAT(officialTestCases[1].description(), Eq("A = 3, B = 4"));
}

TEST_F(MagicTests, SAMPLE_CASE_ValidWithoutGroups) {
    SAMPLE_CASE_Tester tester;
    tester.testValidWithoutGroups();
    TestSuite testSuite = tester.build();

    vector<SampleTestCase> sampleTestCases = testSuite.sampleTests();

    ASSERT_THAT(sampleTestCases, SizeIs(2));
    EXPECT_THAT(sampleTestCases[0], Eq(SampleTestCase("10 20\n30\n", {-1})));
    EXPECT_THAT(sampleTestCases[1], Eq(SampleTestCase("40 50\n60\n", {-1})));
}

TEST_F(MagicTests, SAMPLE_CASE_ValidWithGroups) {
    SAMPLE_CASE_Tester tester;
    tester.testValidWithGroups();
    TestSuite testSuite = tester.build();

    vector<SampleTestCase> sampleTestCases = testSuite.sampleTests();

    ASSERT_THAT(sampleTestCases, SizeIs(2));
    EXPECT_THAT(sampleTestCases[0], Eq(SampleTestCase("10 20\n30\n", {1, 2})));
    EXPECT_THAT(sampleTestCases[1], Eq(SampleTestCase("40 50\n60\n", {2})));
}

TEST_F(MagicTests, LINE_Valid) {
    LINE_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    vector<IOSegment*> segments = ioFormat.inputFormat();

    ASSERT_THAT(segments, SizeIs(3));
    ASSERT_THAT(segments[0]->type(), Eq(IOSegmentType::LINE));
    LineIOSegment* segment0 = (LineIOSegment*) segments[0];
    EXPECT_THAT(segment0->variables(), ElementsAre(
            LineIOSegmentVariable(new FakeVariable("A", VariableType::SCALAR))));
    ASSERT_THAT(segments[1]->type(), Eq(IOSegmentType::LINE));
    LineIOSegment* segment1 = (LineIOSegment*) segments[1];
    EXPECT_THAT(segment1->variables(), ElementsAre(
            LineIOSegmentVariable(new FakeVariable("A", VariableType::SCALAR)),
            LineIOSegmentVariable(new FakeVariable("B", VariableType::SCALAR))));
    LineIOSegment* segment2 = (LineIOSegment*) segments[2];
    EXPECT_THAT(segment2->variables(), ElementsAre(
            LineIOSegmentVariable(new FakeVariable("A", VariableType::SCALAR)),
            LineIOSegmentVariable(new FakeVariable("B", VariableType::SCALAR)),
            LineIOSegmentVariable(new FakeVariable("C", VariableType::VECTOR), 3),
            LineIOSegmentVariable(new FakeVariable("D", VariableType::VECTOR))));
}

}
