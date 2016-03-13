#include "gtest/gtest.h"

#include "tcframe/ioformat/IOFormatException.hpp"
#include "tcframe/ioformat/LineIOSegment.hpp"
#include "tcframe/ioformat/VectorSize.hpp"
#include "tcframe/ioformat/VectorWithSize.hpp"
#include "tcframe/iovariable/ParsingException.hpp"
#include "tcframe/iovariable/PrintingException.hpp"

#include <set>
#include <sstream>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::set;
using std::vector;

using tcframe::IOFormatException;
using tcframe::LineIOSegment;
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
