#include "gtest/gtest.h"

#include "tcframe/io.hpp"

#include <set>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream;
using std::set;
using std::string;
using std::vector;
using tcframe::IOFormat;
using tcframe::IOFormatException;
using tcframe::IOFormatsCollector;
using tcframe::IOMode;
using tcframe::GridIOSegment;
using tcframe::LineIOSegment;
using tcframe::LinesIOSegment;
using tcframe::MatrixSizes;
using tcframe::VectorSize;
using tcframe::VectorWithSize;

TEST(LineIOSegmentTest, UnsupportedType) {
    set<int> S;

    LineIOSegment segment("S");

    try {
        segment, S;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `S` unsatisfied. Expected: basic scalar or string type", e.getMessage());
    }
}

TEST(LineIOSegmentTest, VectorSizeMismatch) {
    int N;
    vector<int> V;

    LineIOSegment segment("V");
    segment, V % VectorSize(N);

    N = 4;
    V = vector<int>{1, 2, 3};

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Number of elements of vector `V` unsatisfied. Expected: 4, actual: 3", e.getMessage());
    }
};

TEST(LineIOSegmentTest, FailedParsingBecauseNoSpace) {
    int A, B;

    LineIOSegment segment("A, B");
    segment, A, B;

    istringstream sin("7\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <space> after variable `A`", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedParsingBecauseNoNewLine) {
    int A, B;

    LineIOSegment segment("A, B");
    segment, A, B;

    istringstream sin("7 123");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <new line> after variable `B`", e.getMessage());
    }
}

TEST(LineIOSegmentTest, EmptyLinePrinting) {
    LineIOSegment segment("");

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(LineIOSegmentTest, SingleScalarPrinting) {
    int X;

    LineIOSegment segment("X");
    segment, X;

    X = 42;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42\n", sout.str());
}

TEST(LineIOSegmentTest, SingleScalarParsing) {
    int X;

    LineIOSegment segment("X");
    segment, X;

    istringstream sin("42\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, X);
}

TEST(LineIOSegmentTest, MultipleScalarsPrinting) {
    int A, B, C;

    LineIOSegment segment("A, B, C");
    segment, A, B, C;

    A = 42;
    B = 7;
    C = 123;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42 7 123\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleScalarsParsing) {
    int A, B, C;

    LineIOSegment segment("A, B, C");
    segment, A, B, C;

    istringstream sin("42 7 123\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, A);
    EXPECT_EQ(7, B);
    EXPECT_EQ(123, C);
}

TEST(LineIOSegmentTest, SingleVectorPrinting) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V % VectorSize(3);

    V = vector<int>{1, 2, 3};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
};

TEST(LineIOSegmentTest, SingleVectorParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V % VectorSize(3);

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
};

TEST(LineIOSegmentTest, MultipleVectorsPrinting) {
    vector<int> V, W;

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3 4 5 6 7\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleVectorsParsing) {
    vector<int> V, W;

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    istringstream sin("1 2 3 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
}

TEST(LineIOSegmentTest, MixedVariablesPrinting) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    A = V.size();
    B = W.size();

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(LineIOSegmentTest, MixedVariablesParsing) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    A = V.size();
    B = W.size();

    istringstream sin("3 1 2 3 4 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
    EXPECT_EQ(3, A);
    EXPECT_EQ(4, B);
}

TEST(LinesIOSegmentTest, UnsupportedType) {
    int X;

    LinesIOSegment segment("X");

    try {
        segment, X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `X` unsatisfied. Expected: vector of basic scalar or string type", e.getMessage());
    }

    vector<vector<int>> V;

    segment = LinesIOSegment("V");

    try {
        segment, V;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `V` unsatisfied. Expected: vector of basic scalar or string type", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, NoVariables) {
    LinesIOSegment segment("");

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch(IOFormatException& e) {
        EXPECT_EQ("Lines segment must have at least one variable", e.getMessage());
    }
}

TEST(LineIOSegmentTest, NoVectorSizes) {
    vector<int> V;

    LinesIOSegment segment("V");
    segment, V;

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch(IOFormatException& e) {
        EXPECT_EQ("Lines segment must define vector sizes", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, VectorSizesMismatch) {
    int N;
    vector<int> V, W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(N);

    N = 3;
    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Number of elements of vector `W` unsatisfied. Expected: 3, actual: 4", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingBecauseNoSpace) {
    vector<int> V, W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 2\n3\n5 6\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <space> after variable `V[1]`", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingBecauseNoNewLine) {
    vector<int> V, W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 2\n3 4\n5 6");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <new line> after variable `W[2]`", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, SingleVectorPrinting) {
    vector<int> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

    V = vector<int>{1, 2, 3};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1\n2\n3\n", sout.str());
}

TEST(LinesIOSegmentTest, SingleVectorParsing) {
    vector<int> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1\n2\n3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(LinesIOSegmentTest, MultipleVectorsPrinting) {
    vector<int> V;
    vector<string> W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    V = vector<int>{1, 2, 3};
    W = vector<string>{"a", "bb", "ccc"};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 a\n2 bb\n3 ccc\n", sout.str());
}

TEST(LinesIOSegmentTest, MultipleVectorsParsing) {
    vector<int> V;
    vector<string> W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    V = vector<int>{1, 2, 3};
    W = vector<string>{"a", "bb", "ccc"};

    istringstream sin("1 a\n2 bb\n3 ccc\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<string>{"a", "bb", "ccc"}), W);
}

TEST(GridIOSegmentTest, UnsupportedTypes) {
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

TEST(GridIOSegmentTest, NonSingularVariables) {
    GridIOSegment segment("");

    ostringstream sout;

    try {
        segment.printTo(sout);
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

TEST(GridIOSegmentTest, NoMatrixSizes) {
    vector<vector<int>> G;

    GridIOSegment segment("G");
    segment, G;

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Grid segment must define matrix sizes", e.getMessage());
    }
}

TEST(GridIOSegmentTest, RowSizesMismatch) {
    int N, M;
    vector<vector<int>> G;

    GridIOSegment segment("G");
    (segment, G) % MatrixSizes(N, M);

    N = 3;
    M = 3;
    G = vector<vector<int>>{ {1, 2}, {3, 4, 5} };

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Number of rows of matrix `G` unsatisfied. Expected: 3, actual: 2", e.getMessage());
    }
}

TEST(GridIOSegmentTest, ColumnSizesMismatch) {
    int N, M;
    vector<vector<int>> G;

    GridIOSegment segment("G");
    (segment, G) % MatrixSizes(N, M);

    N = 2;
    M = 3;
    G = vector<vector<int>>{ {1, 2, 3}, {4, 5} };

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Number of columns row 1 of matrix `G` (0-based) unsatisfied. Expected: 3, actual: 2", e.getMessage());
    }
}

TEST(GridIOSegmentTest, FailedParsingBecauseNoSpace) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <space> after variable `C[1][0]`", e.getMessage());
    }
}

TEST(GridIOSegmentTest, FailedParsingBecauseNoNewLine) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3 4");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Expected: <new line> after variable `C[1][1]`", e.getMessage());
    }
}

TEST(GridIOSegmentTest, CharPrinting) {
    vector<vector<char>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    C = vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} };

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("ab\ncd\n", sout.str());
}

TEST(GridIOSegmentTest, CharParsing) {
    vector<vector<char>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("ab\ncd\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} }), C);
}

TEST(GridIOSegmentTest, NonCharPrinting) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    C = vector<vector<int>>{ {1, 2}, {3, 4} };

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2\n3 4\n", sout.str());
}

TEST(GridIOSegmentTest, NonCharParsing) {
    vector<vector<int>> C;

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

    istringstream sin("1 2\n3 4\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<int>>{ {1, 2}, {3, 4} }), C);
}

TEST(IOFormatTest, MultipleLinesPrinting) {
    int A, B;
    int K;

    IOFormat format;

    LineIOSegment segment1("A, B");
    segment1, A, B;

    LineIOSegment segment2("K");
    segment2, K;

    format.addSegment(&segment1);
    format.addSegment(&segment2);

    A = 1;
    B = 2;
    K = 77;

    ostringstream sout;
    format.printTo(sout);

    EXPECT_EQ("1 2\n77\n", sout.str());
}

TEST(IOFormatsCollectorTest, InputFormatCollection) {
    int A, B;
    int K;

    IOFormatsCollector collector;

    collector.addLineSegment("A, B"), A, B;
    collector.addLineSegment("K"), K;

    A = 1;
    B = 2;
    K = 77;

    IOFormat* format = collector.collectFormat(IOMode::INPUT);

    ostringstream sout;
    format->printTo(sout);

    EXPECT_EQ("1 2\n77\n", sout.str());
}
