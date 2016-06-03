#include "gmock/gmock.h"

#include "tcframe/variable/Matrix.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class MatrixTests : public Test {
protected:
    vector<vector<int>> m;
    vector<vector<char>> c;
    Matrix* M = Matrix::create(m, "M");
    Matrix* C = Matrix::create(c, "C");
};

TEST_F(MatrixTests, Parsing_Successful) {
    istringstream in("1 2 3\n4 5 6\n");
    M->parseFrom(&in, 2, 3);

    EXPECT_THAT(m, Eq(vector<vector<int>>{{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(MatrixTests, Parsing_Char) {
    istringstream in("abc\ndef\n");
    C->parseFrom(&in, 2, 3);

    EXPECT_THAT(c, Eq(vector<vector<char>>{{'a', 'b', 'c'}, {'d', 'e', 'f'}}));
}

TEST_F(MatrixTests, Parsing_Failed_MissingSpace) {
    istringstream in("1 2 3\n4\n");

    try {
        M->parseFrom(&in, 2, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'M[1][0]'"));
    }
}

TEST_F(MatrixTests, Parsing_Failed_MissingNewline) {
    istringstream in("1 2 3 4 5 6\n");

    try {
        M->parseFrom(&in, 2, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'M[0][2]'"));
    }
}

TEST_F(MatrixTests, Parsing_Failed_ExtraWhitespace) {
    istringstream in("1 2 3\n 4 5 6\n");

    try {
        M->parseFrom(&in, 2, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'M[1][0]'. Found: <whitespace>"));
    }
}

TEST_F(MatrixTests, Parsing_WithoutSize_Rows_Successful) {
    istringstream in("1 2\n3 4 5\n6\n");
    M->parseAndAddRowFrom(&in, 0);
    WhitespaceManipulator::parseNewline(&in, "");
    M->parseAndAddRowFrom(&in, 1);
    WhitespaceManipulator::parseNewline(&in, "");
    M->parseAndAddRowFrom(&in, 2);
    WhitespaceManipulator::parseNewline(&in, "");

    EXPECT_THAT(m, Eq(vector<vector<int>>{{1, 2}, {3, 4, 5}, {6}}));
}

TEST_F(MatrixTests, Parsing_WithoutSize_Rows_Failed_LeadingWhitespace) {
    istringstream in(" 1 2 3");

    try {
        M->parseAndAddRowFrom(&in, 0);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'M[0][0]'. Found: <whitespace>"));
    }
}

TEST_F(MatrixTests, Parsing_WithoutSize_Rows_Failed_MissingVariable) {
    istringstream in("1 2  3");

    try {
        M->parseAndAddRowFrom(&in, 0);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'M[0][2]'. Found: <whitespace>"));
    }
}

TEST_F(MatrixTests, Printing_WithoutSize) {
    m = {{1, 2, 3}, {4, 5, 6}};
    ostringstream out;
    M->printTo(&out);

    EXPECT_THAT(out.str(), Eq("1 2 3\n4 5 6\n"));
}

TEST_F(MatrixTests, Printing_WithoutSize_Rows) {
    m = {{1, 2, 3}, {4, 5, 6}};
    ostringstream out;
    M->printRowTo(0, &out);
    out << endl;
    M->printRowTo(1, &out);
    out << endl;

    EXPECT_THAT(out.str(), Eq("1 2 3\n4 5 6\n"));
}

TEST_F(MatrixTests, Printing_Char) {
    c = {{'a', 'b', 'c'}, {'d', 'e', 'f'}};
    ostringstream out;
    C->printTo(&out);

    EXPECT_THAT(out.str(), Eq("abc\ndef\n"));
}

}
