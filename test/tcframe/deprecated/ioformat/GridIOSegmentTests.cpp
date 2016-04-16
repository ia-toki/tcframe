#include "gtest/gtest.h"

#include "tcframe/deprecated/ioformat/IOFormatException.hpp"
#include "tcframe/deprecated/ioformat/GridIOSegment.hpp"
#include "tcframe/deprecated/ioformat/MatrixSizes.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"

#include <sstream>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::vector;

using tcframe::GridIOSegment;
using tcframe::IOFormatException;
using tcframe::MatrixSizes;
using tcframe::ParsingException;
using tcframe::PrintingException;

TEST(DeprecatedGridIOSegmentTest, UnsupportedTypes) {
    int X;

    GridIOSegment segment("X");

    try {
        segment, X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `X` unsatisfied. Expected: matrix of basic scalar or string type", e.getMessage());
    }

    vector<vector<vector<int>>> V;

    segment = GridIOSegment("V");

    try {
        segment, V;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `V` unsatisfied. Expected: matrix of basic scalar or string type", e.getMessage());
    }

}

TEST(DeprecatedGridIOSegmentTest, NonSingularVariables) {
    GridIOSegment segment("");

    segment % MatrixSizes(2, 3);

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Grid segment must have exactly one variable", e.getMessage());
    }

    vector<vector<int>> V, W;

    segment = GridIOSegment("V, W");

    try {
        segment, V, W;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Grid segment must have exactly one variable", e.getMessage());
    }

}

TEST(DeprecatedGridIOSegmentTest, NoMatrixSizes) {
    vector<vector<int>> G;

    GridIOSegment segment("G");
    segment, G;

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Grid segment must define matrix sizes", e.getMessage());
    }
}

TEST(DeprecatedGridIOSegmentTest, FailedPrintingBecauseRowSizesMismatch) {
    int N = 3;
    int M = 3;
    vector<vector<int>> G = { {1, 2}, {3, 4, 5} };

    GridIOSegment segment("G");
    (segment, G) % MatrixSizes(N, M);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
        EXPECT_EQ("Number of rows of matrix `G` unsatisfied. Expected: 3, actual: 2", e.getMessage());
    }
}

TEST(DeprecatedGridIOSegmentTest, FailedPrintingBecauseColumnSizesMismatch) {
    int N = 2;
    int M = 3;
    vector<vector<int>> G = { {1, 2, 3}, {4, 5} };

    GridIOSegment segment("G");
    (segment, G) % MatrixSizes(N, M);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
        EXPECT_EQ("Number of columns row 1 of matrix `G` (0-based) unsatisfied. Expected: 3, actual: 2", e.getMessage());
    }
}

TEST(DeprecatedGridIOSegmentTest, FailedParsingBecauseNoSpace) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> after variable `C[1][0]`", e.getMessage());
    }
}

TEST(DeprecatedGridIOSegmentTest, FailedParsingBecauseNoNewLine) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3 4");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <new line> after variable `C[1][1]`", e.getMessage());
    }
}

TEST(DeprecatedGridIOSegmentTest, CharPrinting) {
    vector<vector<char>> C = { {'a', 'b'}, {'c', 'd'} };

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("ab\ncd\n", sout.str());
}

TEST(DeprecatedGridIOSegmentTest, CharParsing) {
    vector<vector<char>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("ab\ncd\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} }), C);
}

TEST(DeprecatedGridIOSegmentTest, NonCharPrinting) {
    vector<vector<int>> C = { {1, 2}, {3, 4} };

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2\n3 4\n", sout.str());
}

TEST(DeprecatedGridIOSegmentTest, NonCharParsing) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3 4\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<int>>{ {1, 2}, {3, 4} }), C);
}
