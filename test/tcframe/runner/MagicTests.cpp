#include "gmock/gmock.h"

#include "../variable/FakeVariable.hpp"
#include "tcframe/experimental/runner.hpp"

using ::testing::Eq;
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
        void testValid() {
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

    class LINES_Tester : public IOFormatBuilder {
    protected:
        vector<int> X, Y;
        vector<vector<int>> Z;

    public:
        void testValid() {
            LINES(X) % SIZE(2);
            LINES(X, Y) % SIZE(3);
            LINES(X, Y, Z) % SIZE(4);
        }
    };

    class GRID_Tester : public IOFormatBuilder {
    protected:
        vector<vector<int>> M;

    public:
        void testValid() {
            GRID(M) % SIZE(2, 3);
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

    EXPECT_THAT(constraintSuite, ConstraintSuiteBuilder()
            .addConstraint(Constraint([]{return true;}, "1 <= A && A <= 100"))
            .addConstraint(Constraint([]{return true;}, "1 <= B && B <= 100"))
            .build());
}

TEST_F(MagicTests, CASE_Valid) {
    CASE_Tester tester;
    tester.testValid();
    TestSuite testSuite = tester.build();

    EXPECT_THAT(testSuite, Eq(TestSuiteBuilder()
            .addOfficialTestCase(OfficialTestCase([]{}, "A = 1, B = 2"))
            .addOfficialTestCase(OfficialTestCase([]{}, "A = 3, B = 4"))
            .build()));
}

TEST_F(MagicTests, SAMPLE_CASE_Valid) {
    SAMPLE_CASE_Tester tester;
    tester.testValid();
    TestSuite testSuite = tester.build();

    EXPECT_THAT(testSuite, Eq(TestSuiteBuilder()
              .addSampleTestCase({"10 20", "30"})
              .addSampleTestCase({"40 50", "60"})
              .build()));
}

TEST_F(MagicTests, SAMPLE_CASE_WithGroups_Valid) {
    SAMPLE_CASE_Tester tester;
    tester.testValidWithGroups();
    TestSuite testSuite = tester.build();

    EXPECT_THAT(testSuite, Eq(TestSuiteBuilder()
            .addSampleTestCase({"10 20", "30"}, {1, 2})
            .addSampleTestCase({"40 50", "60"}, {2})
            .build()));
}

TEST_F(MagicTests, LINE_Valid) {
    LINE_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    int dummy;
    vector<int> dummy2;
    IOFormatBuilder builder;
    builder.prepareForInputFormat();
    builder.newLineIOSegment()
            .addScalarVariable(Scalar::create(dummy, "A"));
    builder.newLineIOSegment()
            .addScalarVariable(Scalar::create(dummy, "A"))
            .addScalarVariable(Scalar::create(dummy, "B"));
    builder.newLineIOSegment()
            .addScalarVariable(Scalar::create(dummy, "A"))
            .addScalarVariable(Scalar::create(dummy, "B"))
            .addVectorVariable(Vector::create(dummy2, "C"), 3)
            .addVectorVariable(Vector::create(dummy2, "D"));

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, LINES_Valid) {
    LINES_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    vector<int> dummy;
    vector<vector<int>> dummy2;
    IOFormatBuilder builder;
    builder.prepareForInputFormat();
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "X"))
            .setSize(2);
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "X"))
            .addVectorVariable(Vector::create(dummy, "Y"))
            .setSize(3);
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "X"))
            .addVectorVariable(Vector::create(dummy, "Y"))
            .addJaggedVectorVariable(Matrix::create(dummy2, "Z"))
            .setSize(4);

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, GRID_Valid) {
    GRID_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    vector<vector<int>> dummy;
    IOFormatBuilder builder;
    builder.prepareForInputFormat();
    builder.newGridIOSegment()
            .addMatrixVariable(Matrix::create(dummy, "M"))
            .setSize(2, 3);

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

}
