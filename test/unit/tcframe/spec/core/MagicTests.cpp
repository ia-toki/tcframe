#include "gmock/gmock.h"

#include "tcframe/runner.hpp"

using ::testing::Eq;
using ::testing::StrEq;
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

    class LINE_Tester : public IOFormatBuilder {
    protected:
        int N = 3;
        int A, B;
        vector<int> C, D;

        vector<vector<int>> bogus;

    public:
        void testValid() {
            LINE(A);
            EMPTY_LINE();
            LINE(A, B);
            LINE(C % SIZE(N));
            LINE(A, B, C % SIZE(3), D);
        }

        void testInvalid() {
            LINE(bogus);
        }
    };

    class LINES_Tester : public IOFormatBuilder {
    protected:
        int N = 4;
        vector<int> X, Y;
        vector<vector<int>> Z;
        vector<int> A, B;

        int bogus;

    public:
        void testValid() {
            LINES(X) % SIZE(2);
            LINES(X, Y) % SIZE(3);
            LINES(X, Y, Z) % SIZE(N);
            LINES(A, B);
        }

        void testInvalid() {
            LINES(bogus);
        }
    };

    class RAW_LINE_Tester : public IOFormatBuilder {
    protected:
        string S;

        int bogus;

    public:
        void testValid() {
            RAW_LINE(S);
        }

        void testInvalid() {
            RAW_LINE(bogus);
        }
    };

    class RAW_LINES_Tester : public IOFormatBuilder {
    protected:
        int N = 3;
        vector<string> X, Y, Z;

        vector<int> bogus;

    public:
        void testValid() {
            RAW_LINES(X) % SIZE(2);
            RAW_LINES(Y) % SIZE(N);
            RAW_LINES(Z);
        }

        void testInvalid() {
            RAW_LINES(bogus);
        }
    };

    class GRID_Tester : public IOFormatBuilder {
    protected:
        int R = 2;
        int C = 3;
        vector<vector<int>> M1, M2, M3, M4;

        int bogus;

    public:
        void testValid() {
            GRID(M1) % SIZE(2, 3);
            GRID(M2) % SIZE(R, 3);
            GRID(M3) % SIZE(2, C);
            GRID(M4) % SIZE(R, C);
        }

        void testInvalid() {
            GRID(bogus);
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
            .addConstraint([]{return true;}, "1 <= A && A <= 100")
            .addConstraint([]{return true;}, "1 <= B && B <= 100")
            .build());
}

TEST_F(MagicTests, CASE_Valid) {
    CASE_Tester tester;
    tester.testValid();
    TestSuite testSuite = tester.build();

    EXPECT_THAT(testSuite, Eq(TestSuiteBuilder()
            .addOfficialTestCase([]{}, "A = 1, B = 2")
            .addOfficialTestCase([]{}, "A = 3, B = 4")
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
    builder.newLineIOSegment();
    builder.newLineIOSegment()
            .addScalarVariable(Scalar::create(dummy, "A"))
            .addScalarVariable(Scalar::create(dummy, "B"));
    builder.newLineIOSegment()
            .addVectorVariable(Vector::create(dummy2, "C"), [] {return 3;});
    builder.newLineIOSegment()
            .addScalarVariable(Scalar::create(dummy, "A"))
            .addScalarVariable(Scalar::create(dummy, "B"))
            .addVectorVariable(Vector::create(dummy2, "C"), [] {return 3;})
            .addVectorVariable(Vector::create(dummy2, "D"));

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, LINE_Invalid) {
    LINE_Tester tester;
    tester.prepareForInputFormat();

    try {
        tester.testInvalid();
        tester.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The type of variable 'bogus' is not supported for a line segment"));
    }
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
            .setSize([] {return 2;});
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "X"))
            .addVectorVariable(Vector::create(dummy, "Y"))
            .setSize([] {return 3;});
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "X"))
            .addVectorVariable(Vector::create(dummy, "Y"))
            .addJaggedVectorVariable(Matrix::create(dummy2, "Z"))
            .setSize([] {return 4;});
    builder.newLinesIOSegment()
            .addVectorVariable(Vector::create(dummy, "A"))
            .addVectorVariable(Vector::create(dummy, "B"))
            .build();

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, LINES_Invalid) {
    LINES_Tester tester;
    tester.prepareForInputFormat();

    try {
        tester.testInvalid();
        tester.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The type of variable 'bogus' is not supported for a lines segment"));
    }
}

TEST_F(MagicTests, RAW_LINE_Valid) {
    RAW_LINE_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    string dummy;
    IOFormatBuilder builder;
    builder.prepareForInputFormat();
    builder.newRawLineIOSegment()
            .addScalarVariable(Scalar::createRaw(dummy, "S"));

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, RAW_LINE_Invalid) {
    RAW_LINE_Tester tester;
    tester.prepareForInputFormat();

    try {
        tester.testInvalid();
        tester.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The type of variable 'bogus' is not supported for a raw line segment"));
    }
}

TEST_F(MagicTests, RAW_LINES_Valid) {
    RAW_LINES_Tester tester;
    tester.prepareForInputFormat();
    tester.testValid();
    IOFormat ioFormat = tester.build();

    vector<string> dummy;
    IOFormatBuilder builder;
    builder.prepareForInputFormat();
    builder.newRawLinesIOSegment()
            .addVectorVariable(Vector::createRaw(dummy, "X"))
            .setSize([] {return 2;});
    builder.newRawLinesIOSegment()
            .addVectorVariable(Vector::createRaw(dummy, "Y"))
            .setSize([] {return 3;});
    builder.newRawLinesIOSegment()
            .addVectorVariable(Vector::createRaw(dummy, "Z"));

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, RAW_LINES_Invalid) {
    RAW_LINES_Tester tester;
    tester.prepareForInputFormat();

    try {
        tester.testInvalid();
        tester.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The type of variable 'bogus' is not supported for a raw lines segment"));
    }
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
            .addMatrixVariable(Matrix::create(dummy, "M1"))
            .setSize([] {return 2;}, [] {return 3;});
    builder.newGridIOSegment()
            .addMatrixVariable(Matrix::create(dummy, "M2"))
            .setSize([] {return 2;}, [] {return 3;});
    builder.newGridIOSegment()
            .addMatrixVariable(Matrix::create(dummy, "M3"))
            .setSize([] {return 2;}, [] {return 3;});
    builder.newGridIOSegment()
            .addMatrixVariable(Matrix::create(dummy, "M4"))
            .setSize([] {return 2;}, [] {return 3;});

    EXPECT_THAT(ioFormat, Eq(builder.build()));
}

TEST_F(MagicTests, GRID_Invalid) {
    GRID_Tester tester;
    tester.prepareForInputFormat();

    try {
        tester.testInvalid();
        tester.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("The type of variable 'bogus' is not supported for a grid segment"));
    }
}

}
