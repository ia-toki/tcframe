#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/io/LineIOSegmentManipulator.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class LineIOSegmentManipulatorTests : public Test {};

TEST_F(LineIOSegmentManipulatorTests, Parsing_SingleVariable_Successful) {
    int X;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(X, "X"))
            .build();
    istringstream in("42\n");

    LineIOSegmentManipulator::parse(segment, &in);
    EXPECT_THAT(X, Eq(42));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_SingleVariable_Failed_MissingNewline) {
    int X;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(X, "X"))
            .build();
    istringstream in("42");

    EXPECT_THROW({LineIOSegmentManipulator::parse(segment, &in);}, runtime_error);
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_MultipleVariables_Successful) {
    int A, B;
    vector<int> C, D;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .addVectorVariable(Vector::create(C, "C"), 2)
            .addVectorVariable(Vector::create(D, "D"))
            .build();
    istringstream in("42 123 1 2 3 4 5\n");

    LineIOSegmentManipulator::parse(segment, &in);
    EXPECT_THAT(A, Eq(42));
    EXPECT_THAT(B, Eq(123));
    EXPECT_THAT(C, ElementsAre(1, 2));
    EXPECT_THAT(D, ElementsAre(3, 4, 5));
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_MultipleVariables_Failed_MissingSpace) {
    int A, B;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .build();
    istringstream in("42");

    EXPECT_THROW({LineIOSegmentManipulator::parse(segment, &in);}, runtime_error);
}

TEST_F(LineIOSegmentManipulatorTests, Parsing_MultipleVariables_Failed_MissingVariable) {
    int A, B;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .build();
    istringstream in("42 ");

    EXPECT_THROW({LineIOSegmentManipulator::parse(segment, &in);}, runtime_error);
}

TEST_F(LineIOSegmentManipulatorTests, Printing_SingleVariable) {
    int X;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(X, "X"))
            .build();
    ostringstream out;

    X = 42;

    LineIOSegmentManipulator::print(segment, &out);
    EXPECT_THAT(out.str(), Eq("42\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_MultipleVariables) {
    int A, B;
    vector<int> C, D;
    LineIOSegment *segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .addScalarVariable(Scalar::create(B, "B"))
            .addVectorVariable(Vector::create(C, "C"), 2)
            .addVectorVariable(Vector::create(D, "D"))
            .build();
    ostringstream out;

    A = 42;
    B = 123;
    C = {1, 2};
    D = {3, 4, 5};

    LineIOSegmentManipulator::print(segment, &out);
    EXPECT_THAT(out.str(), Eq("42 123 1 2 3 4 5\n"));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_Vector_Failed_SizeMismatch) {
    vector<int> V;

    LineIOSegment *segment = LineIOSegmentBuilder()
            .addVectorVariable(Vector::create(V, "V"), 2)
            .build();
    ostringstream out;

    V = {1, 2, 3};

    try {
        LineIOSegmentManipulator::print(segment, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of elements of vector 'V' unsatisfied. Expected: 2, actual: 3"));
    }
}

}
