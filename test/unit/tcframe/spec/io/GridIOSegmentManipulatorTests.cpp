#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/spec/io/GridIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class GridIOSegmentManipulatorTests : public Test {
protected:
    function<int()> rows = [] {return 2;};
    function<int()> columns = [] {return 3;};

    vector<vector<int>> M;
    GridIOSegment* segment = GridIOSegmentBuilder()
            .addMatrixVariable(Matrix::create(M, "M"))
            .setSize(rows, columns)
            .build();
    
    GridIOSegmentManipulator manipulator;
};

TEST_F(GridIOSegmentManipulatorTests, Parsing_Successful) {
    istringstream in("1 2 3\n4 5 6\n");

    manipulator.parse(segment, &in);
    EXPECT_THAT(M, Eq(vector<vector<int>>{{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(GridIOSegmentManipulatorTests, Parsing_Successful_CheckLastVariable) {
    istringstream in("1 2 3\n4 5 6\n");

    EXPECT_THAT(manipulator.parse(segment, &in), Eq("'M[1][2]'"));
}

TEST_F(GridIOSegmentManipulatorTests, Printing_Successful) {
    ostringstream out;

    M = {{1, 2, 3}, {4, 5, 6}};

    manipulator.print(segment, &out);
    EXPECT_THAT(out.str(), Eq("1 2 3\n4 5 6\n"));
}

TEST_F(GridIOSegmentManipulatorTests, Printing_Failed_RowsMismatch) {
    ostringstream out;

    M = {{1, 2, 3}};

    try {
        manipulator.print(segment, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of rows of matrix 'M' unsatisfied. Expected: 2, actual: 1"));
    }
}

TEST_F(GridIOSegmentManipulatorTests, Printing_Failed_ColumnsMismatch) {
    ostringstream out;

    M = {{1, 2, 3}, {4, 5}};

    try {
        manipulator.print(segment, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of columns of row 1 of matrix 'M' unsatisfied. Expected: 3, actual: 2"));
    }
}

}
