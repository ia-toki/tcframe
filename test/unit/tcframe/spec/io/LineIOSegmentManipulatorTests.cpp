#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/spec/io/LineIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class LineIOSegmentManipulatorTests : public Test {
protected:
    int A, B;
    vector<int> C, D;

    function<int()> size = [] {return 2;};
    function<int()> emptySize = [] {return 0;};

    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .build();
    LineIOSegment* segmentWithVector = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .addVectorVariable(Vector::create(C, "C"), size)
            .build();
    LineIOSegment* segmentWithEmptyVector = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .addVectorVariable(Vector::create(C, "C"), emptySize)
            .build();
    LineIOSegment* segmentWithVectorOnly = LineIOSegmentBuilder()
            .addVectorVariable(Vector::create(C, "C"), size)
            .build();
    LineIOSegment* segmentWithEmptyVectorOnly = LineIOSegmentBuilder()
            .addVectorVariable(Vector::create(C, "C"), emptySize)
            .build();
    LineIOSegment* segmentWithVectorWithoutSize = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .addVectorVariable(Vector::create(C, "C"), size)
            .addVectorVariable(Vector::create(D, "D"))
            .build();
    LineIOSegment* segmentWithVectorWithoutSizeOnly = LineIOSegmentBuilder()
            .addVectorVariable(Vector::create(D, "D"))
            .build();
    
    LineIOSegmentManipulator manipulator;
};

TEST_F(LineIOSegmentManipulatorTests, Parsing_EmptyLine) {
    istringstream in("\n");

    manipulator.parse(LineIOSegmentBuilder().build(), &in);
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_Successful) {
    istringstream in("42 123\n");

    manipulator.parse(segment, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_Successful_CheckLastVariable) {
    istringstream in("42 123\n");

    EXPECT_THAT(manipulator.parse(segment, &in), Eq("'B'"));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Successful) {
    istringstream in("42 123 1 2\n");

    manipulator.parse(segmentWithVector, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, Eq(vector<int>{1, 2}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Empty_Successful) {
    istringstream in("42 123\n");

    manipulator.parse(segmentWithEmptyVector, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, Eq(vector<int>{}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Only_Successful) {
    istringstream in("1 2\n");

    manipulator.parse(segmentWithVectorOnly, &in);
    EXPECT_THAT(C, Eq(vector<int>{1, 2}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Only_Empty_Successful) {
    istringstream in("\n");

    manipulator.parse(segmentWithEmptyVectorOnly, &in);
    EXPECT_THAT(C, Eq(vector<int>{}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Successful_CheckLastVariable) {
    istringstream in("42 123 1 2\n");

    EXPECT_THAT(manipulator.parse(segmentWithVector, &in), Eq("'C[1]'"));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Failed_MissingVariable) {
    istringstream in("42  ");

    try {
        manipulator.parse(segmentWithVector, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'B'. Found: <whitespace>"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Failed_MissingWhitespace) {
    istringstream in("42 123\n");

    try {
        manipulator.parse(segmentWithVector, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'B'"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Failed_MissingNewline) {
    istringstream in("42 123 1 2");

    try {
        manipulator.parse(segmentWithVector, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'C[1]'"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVector_Failed_TooManyElements) {
    istringstream in("42 123 1 2 3 4 5\n");

    try {
        manipulator.parse(segmentWithVector, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'C[1]'"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Successful) {
    istringstream in("42 123 1 2 3 4 5\n");

    manipulator.parse(segmentWithVectorWithoutSize, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, Eq(vector<int>{1, 2}));
    EXPECT_THAT(D, Eq(vector<int>{3, 4, 5}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Empty_Successful) {
    istringstream in("42 123 1 2\n");

    manipulator.parse(segmentWithVectorWithoutSize, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, Eq(vector<int>{1, 2}));
    EXPECT_THAT(D, Eq(vector<int>{}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Only_Successful) {
    istringstream in("3 4 5\n");

    manipulator.parse(segmentWithVectorWithoutSizeOnly, &in);
    EXPECT_THAT(D, Eq(vector<int>{3, 4, 5}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Only_Empty_Successful) {
    istringstream in("\n");

    manipulator.parse(segmentWithVectorWithoutSizeOnly, &in);
    EXPECT_THAT(D, Eq(vector<int>{}));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Successful_CheckLastVariable) {
    istringstream in("42 123 1 2 3 4 5\n");

    EXPECT_THAT(manipulator.parse(segmentWithVectorWithoutSize, &in), Eq("'D[2]'"));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_WithVectorWithoutSize_Failed_MissingSpaceOrNewline) {
    istringstream in("42 123 1 2 3 4 5");

    try {
        manipulator.parse(segmentWithVectorWithoutSize, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> or <newline> after 'D[2]'"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Printing_EmptyLine) {
    ostringstream out;

    manipulator.print(LineIOSegmentBuilder().build(), &out);
    EXPECT_THAT(out.str(), Eq("\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVector_Successful) {
    ostringstream out;

    A = 42;
    B = 123;
    C = {1, 2};

    manipulator.print(segmentWithVector, &out);
    EXPECT_THAT(out.str(), Eq("42 123 1 2\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVector_Empty_Successful) {
    ostringstream out;

    A = 42;
    B = 123;
    C = {};

    manipulator.print(segmentWithEmptyVector, &out);
    EXPECT_THAT(out.str(), Eq("42 123\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVector_Only_Successful) {
    ostringstream out;

    C = {1, 2};

    manipulator.print(segmentWithVectorOnly, &out);
    EXPECT_THAT(out.str(), Eq("1 2\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVector_Only_Empty_Successful) {
    ostringstream out;

    C = {};

    manipulator.print(segmentWithEmptyVectorOnly, &out);
    EXPECT_THAT(out.str(), Eq("\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVector_Failed_SizeMismatch) {
    ostringstream out;

    C = {1, 2, 3};

    try {
        manipulator.print(segmentWithVector, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of elements of vector 'C' unsatisfied. Expected: 2, actual: 3"));
    }
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVectorWithoutSize_Successful) {
    ostringstream out;

    A = 42;
    B = 123;
    C = {1, 2};
    D = {3, 4, 5};

    manipulator.print(segmentWithVectorWithoutSize, &out);
    EXPECT_THAT(out.str(), Eq("42 123 1 2 3 4 5\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVectorWithoutSize_Empty_Successful) {
    ostringstream out;

    A = 42;
    B = 123;
    C = {1, 2};
    D = {};

    manipulator.print(segmentWithVectorWithoutSize, &out);
    EXPECT_THAT(out.str(), Eq("42 123 1 2\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVectorWithoutSize_Only_Successful) {
    ostringstream out;

    D = {3, 4, 5};

    manipulator.print(segmentWithVectorWithoutSizeOnly, &out);
    EXPECT_THAT(out.str(), Eq("3 4 5\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_WithVectorWithoutSize_Only_Empty_Successful) {
    ostringstream out;

    D = {};

    manipulator.print(segmentWithVectorWithoutSizeOnly, &out);
    EXPECT_THAT(out.str(), Eq("\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_ClearVectorSegment_Successful) {
    istringstream in("42 123 1 2\n");

    manipulator.parse(segmentWithVector, &in);

    in.seekg(0, in.beg);
    manipulator.parse(segmentWithVector, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, Eq(vector<int>{1, 2}));
}

}
