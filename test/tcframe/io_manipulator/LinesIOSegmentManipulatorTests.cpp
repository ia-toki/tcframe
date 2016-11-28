#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/io_manipulator/LinesIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class LinesIOSegmentManipulatorTests : public Test {
protected:
    vector<int> X, Y;
    vector<vector<int>> Z;
    int* size = new int(3);

    LinesIOSegment* segment = LinesIOSegmentBuilder()
            .addVectorVariable(Vector::create(X, "X"))
            .addVectorVariable(Vector::create(Y, "Y"))
            .setSize(size)
            .build();
    LinesIOSegment* segmentWithJaggedVector = LinesIOSegmentBuilder()
            .addVectorVariable(Vector::create(X, "X"))
            .addVectorVariable(Vector::create(Y, "Y"))
            .addJaggedVectorVariable(Matrix::create(Z, "Z"))
            .setSize(size)
            .build();
};

TEST_F(LinesIOSegmentManipulatorTests, Parsing_Successful) {
    istringstream in("1 2\n3 4\n5 6\n");

    LinesIOSegmentManipulator::parse(segment, &in);
    EXPECT_THAT(X, Eq(vector<int>{1, 3, 5}));
    EXPECT_THAT(Y, Eq(vector<int>{2, 4, 6}));
}

TEST_F(LinesIOSegmentManipulatorTests, Parsing_Failed_MissingVariable) {
    istringstream in("1 2\n3  ");

    try {
        LinesIOSegmentManipulator::parse(segment, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'Y[1]'. Found: <whitespace>"));
    }
}

TEST_F(LinesIOSegmentManipulatorTests, Parsing_Failed_MissingWhitespace) {
    istringstream in("1 2\n3");

    try {
        LinesIOSegmentManipulator::parse(segment, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'X[1]'"));
    }
}

TEST_F(LinesIOSegmentManipulatorTests, Parsing_Failed_MissingNewline) {
    istringstream in("1 2\n3 4 ");

    try {
        LinesIOSegmentManipulator::parse(segment, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'Y[1]'"));
    }
}

TEST_F(LinesIOSegmentManipulatorTests, Parsing_WithJaggedVector_Successful) {
    istringstream in("1 2 10\n3 4\n5 6 20 30\n");

    LinesIOSegmentManipulator::parse(segmentWithJaggedVector, &in);
    EXPECT_THAT(X, Eq(vector<int>{1, 3, 5}));
    EXPECT_THAT(Y, Eq(vector<int>{2, 4, 6}));
    EXPECT_THAT(Z, Eq(vector<vector<int>>{{10}, {}, {20, 30}}));
}

TEST_F(LinesIOSegmentManipulatorTests, Parsing_WithJaggedVector_Failed_MissingSpaceOrNewline) {
    istringstream in("1 2 10\n3 4\n5 6 20 30");

    try {
        LinesIOSegmentManipulator::parse(segmentWithJaggedVector, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> or <newline> after 'Z[2][1]'"));
    }
}

TEST_F(LinesIOSegmentManipulatorTests, Printing_Successful) {
    ostringstream out;

    X = {1, 3, 5};
    Y = {2, 4, 6};

    LinesIOSegmentManipulator::print(segment, &out);
    EXPECT_THAT(out.str(), Eq("1 2\n3 4\n5 6\n"));
}

TEST_F(LinesIOSegmentManipulatorTests, Printing_Failed_SizeMismatch) {
    ostringstream out;

    X = {1, 3, 5};
    Y = {2, 4};

    try {
        LinesIOSegmentManipulator::print(segment, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of elements of vector 'Y' unsatisfied. Expected: 3, actual: 2"));
    }
}

TEST_F(LinesIOSegmentManipulatorTests, Printing_WithJaggedVector_Successful) {
    ostringstream out;

    X = {1, 3, 5};
    Y = {2, 4, 6};
    Z = {{10}, {}, {20, 30}};

    LinesIOSegmentManipulator::print(segmentWithJaggedVector, &out);
    EXPECT_THAT(out.str(), Eq("1 2 10\n3 4\n5 6 20 30\n"));
}

TEST_F(LinesIOSegmentManipulatorTests, Printing_WithJaggedVector_Failed_SizeMismatch) {
    ostringstream out;

    X = {1, 3, 5};
    Y = {2, 4, 6};
    Z = {{10}, {}};

    try {
        LinesIOSegmentManipulator::print(segmentWithJaggedVector, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of elements of jagged vector 'Z' unsatisfied. Expected: 3, actual: 2"));
    }
}

}
