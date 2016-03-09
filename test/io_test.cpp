#include "gtest/gtest.h"

#include "tcframe/io.hpp"

using std::set;
using tcframe::Failure;
using tcframe::IOFormatException;
using tcframe::IOFormatProvider;
using tcframe::GridIOSegment;
using tcframe::LineIOSegment;
using tcframe::LinesIOSegment;
using tcframe::MatrixSizes;
using tcframe::ParsingException;
using tcframe::PrintingException;
using tcframe::VectorSize;
using tcframe::VectorWithSize;

TEST(LineIOSegmentTest, UnsupportedType) {
    set<int> S;

    LineIOSegment segment("S");

    try {
        segment, S;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `S` unsatisfied. Expected: basic scalar or string type, or vector of those types", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedPrintingBecauseVectorSizeMismatch) {
    int N = 4;
    vector<int> V = {1, 2, 3};

    LineIOSegment segment("V");
    segment, V % VectorSize(N);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
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
    } catch (ParsingException& e) {
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
    } catch (ParsingException& e) {
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
    int X = 42;

    LineIOSegment segment("X");
    segment, X;

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
    int A = 42;
    int B = 7;
    int C = 123;

    LineIOSegment segment("A, B, C");
    segment, A, B, C;

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
    vector<int> V = {1, 2, 3};

    LineIOSegment segment("V");
    segment, V % VectorSize(3);

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
    vector<int> V = {1, 2, 3};
    vector<int> W = {4, 5, 6, 7};

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3 4 5 6 7\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleVectorsParsing) {
    vector<int> V, W;

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    istringstream sin("1 2 3 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
}

TEST(LineIOSegmentTest, MixedVariablesPrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    int A = V.size();
    int B = W.size();

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(LineIOSegmentTest, MixedVariablesParsing) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    istringstream sin("3 1 2 3 4 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
    EXPECT_EQ(3, A);
    EXPECT_EQ(4, B);
}

TEST(LineIOSegmentTest, VectorWithoutSizePrinting) {
    vector<int> V = {1, 2, 3};

    LineIOSegment segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
}

TEST(LineIOSegmentTest, EmptyVectorWithoutSizePrinting) {
    vector<int> V = {};

    LineIOSegment segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(LineIOSegmentTest, VectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(LineIOSegmentTest, EmptyVectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{}), V);
}

TEST(LineIOSegmentTest, MixedWithVectorWithoutSizePrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    int A = V.size();
    int B = W.size();

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(LineIOSegmentTest, MixedWithVectorWithoutSizeParsing) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W;

    istringstream sin("3 1 2 3 4 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
    EXPECT_EQ(3, A);
    EXPECT_EQ(4, B);
}

TEST(LineIOSegmentTest, VectorWithoutSizeNoElementsParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_TRUE(V.empty());
}

TEST(LineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfSpacePrefix) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin(" 1 2 3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[0]`. Found: <whitespace>", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfStrangeCharBetweenElements) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("1 2\t3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[1]`", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfNoNewline) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("1 2 3");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[2]`", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfTrailingSpace) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("1 2 3 ");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[3]`. Found: <EOF>", e.getMessage());
    }
}

TEST(LineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseItIsNotLast) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment("A, V, B, W");

    try {
        segment, A, V, B, W % VectorSize(4);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Vector without size can only be the last variable in a line segment", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, UnsupportedTypes) {
    int X;

    LinesIOSegment segment("X");

    try {
        segment, X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `X` unsatisfied. Expected: (jagged) vector of basic scalar or string type", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, NoVariables) {
    LinesIOSegment segment("");
    segment % VectorSize(4);

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Lines segment must have at least one variable", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, NoVectorSizes) {
    vector<int> V;

    LinesIOSegment segment("V");
    segment, V;

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Lines segment must define vector sizes", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedPrintingBecauseVectorSizesMismatch) {
    int N = 3;
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(N);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
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
    } catch (ParsingException& e) {
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
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <new line> after variable `W[2]`", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, SingleVectorPrinting) {
    vector<int> V = {1, 2, 3};

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

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
    vector<int> V = {1, 2, 3};
    vector<string> W = {"a", "bb", "ccc"};

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 a\n2 bb\n3 ccc\n", sout.str());
}

TEST(LinesIOSegmentTest, MultipleVectorsParsing) {
    vector<int> V;
    vector<string> W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 a\n2 bb\n3 ccc\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<string>{"a", "bb", "ccc"}), W);
}

TEST(LinesIOSegmentTest, JaggedVectorPrinting) {
    vector<vector<int>> V = {{1}, {2, 3}, {4, 5, 6}};

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1\n2 3\n4 5 6\n", sout.str());
}

TEST(LinesIOSegmentTest, JaggedVectorParsing) {
    vector<vector<int>> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1\n2 3\n4 5 6\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<int>>{{1}, {2, 3}, {4, 5, 6}}), V);
}

TEST(LinesIOSegmentTest, FailedParsingJaggedMatrixBecauseOfSpacePrefix) {
    vector<vector<int>> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1 2 3\n 4 5 6\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[1][0]`. Found: <whitespace>", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingJaggedVectorBecauseOfStrangeCharBetweenElements) {
    vector<vector<int>> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2\t3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[0][1]`", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingJaggedVectorBecauseOfNoNewline) {
    vector<vector<int>> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2 3\n4 5 6");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[1][2]`", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingJaggedVectorBecauseOfTrailingSpace) {
    vector<vector<int>> V;

    LinesIOSegment segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2 3\n4 5 6 ");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[1][3]`. Found: <EOF>", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, FailedParsingJaggedVectorBecauseItIsNotLast) {
    vector<vector<int>> V;
    vector<int> W;

    LinesIOSegment segment("V, W");

    try {
        (segment, V, W) % VectorSize(4);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Jagged vector can only be the last variable in a lines segment", e.getMessage());
    }
}

TEST(LinesIOSegmentTest, MixedWithJaggedVectorPrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<vector<int>> W = vector<vector<int>>{{4}, {5, 6}, {7, 8, 9}};

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 4\n2 5 6\n3 7 8 9\n", sout.str());
}

TEST(LinesIOSegmentTest, MixedWithJaggedVectorSizeParsing) {
    vector<int> V;
    vector<vector<int>> W;

    LinesIOSegment segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 4\n2 5 6\n3 7 8 9\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<vector<int>>{{4}, {5, 6}, {7, 8, 9}}), W);
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

TEST(GridIOSegmentTest, NoMatrixSizes) {
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

TEST(GridIOSegmentTest, FailedPrintingBecauseRowSizesMismatch) {
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

TEST(GridIOSegmentTest, FailedPrintingBecauseColumnSizesMismatch) {
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

TEST(GridIOSegmentTest, FailedParsingBecauseNoSpace) {
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

TEST(GridIOSegmentTest, FailedParsingBecauseNoNewLine) {
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

TEST(GridIOSegmentTest, CharPrinting) {
    vector<vector<char>> C = { {'a', 'b'}, {'c', 'd'} };

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

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
    vector<vector<int>> C = { {1, 2}, {3, 4} };

    GridIOSegment segment("C");
    (segment, C) % MatrixSizes(2, 2);

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

TEST(IOFormatProviderTest, Printing) {
    int A = 1;
    int B = 2;
    int K = 77;

    IOFormatProvider provider;

    ostringstream sout;

    provider.beginPrintingFormat(&sout);
    provider.applyLastSegment(); provider.applyLineSegment("A, B"), A, B;
    provider.applyLastSegment(); provider.applyLineSegment("K"), K;
    provider.endPrintingFormat();

    EXPECT_EQ("1 2\n77\n", sout.str());
}

TEST(IOFormatProviderTest, Parsing) {
    int A, B;
    int K;

    IOFormatProvider provider;

    istringstream sin("1 2\n77\n");

    provider.beginParsingFormat(&sin);
    provider.applyLastSegment(); provider.applyLineSegment("A, B"), A, B;
    provider.applyLastSegment(); provider.applyLineSegment("K"), K;
    provider.endPrintingFormat();

    EXPECT_EQ(1, A);
    EXPECT_EQ(2, B);
    EXPECT_EQ(77, K);
}
