#include "gtest/gtest.h"

#include "tcframe/deprecated/ioformat/IOFormatException.hpp"
#include "tcframe/deprecated/ioformat/LineIOSegment.hpp"
#include "tcframe/deprecated/ioformat/VectorSize.hpp"
#include "tcframe/deprecated/ioformat/VectorWithSize.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"

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

TEST(DeprecatedLineIOSegmentTest, UnsupportedType) {
    set<int> S;

    LineIOSegment segment("S");

    try {
        segment, S;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `S` unsatisfied. Expected: basic scalar or string type, or vector of those types", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentTest, FailedPrintingBecauseVectorSizeMismatch) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingBecauseNoSpace) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingBecauseNoNewLine) {
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

TEST(DeprecatedLineIOSegmentTest, EmptyLinePrinting) {
    LineIOSegment segment("");

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, SingleScalarPrinting) {
    int X = 42;

    LineIOSegment segment("X");
    segment, X;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, SingleScalarParsing) {
    int X;

    LineIOSegment segment("X");
    segment, X;

    istringstream sin("42\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, X);
}

TEST(DeprecatedLineIOSegmentTest, MultipleScalarsPrinting) {
    int A = 42;
    int B = 7;
    int C = 123;

    LineIOSegment segment("A, B, C");
    segment, A, B, C;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42 7 123\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, MultipleScalarsParsing) {
    int A, B, C;

    LineIOSegment segment("A, B, C");
    segment, A, B, C;

    istringstream sin("42 7 123\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, A);
    EXPECT_EQ(7, B);
    EXPECT_EQ(123, C);
}

TEST(DeprecatedLineIOSegmentTest, SingleVectorPrinting) {
    vector<int> V = {1, 2, 3};

    LineIOSegment segment("V");
    segment, V % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
};

TEST(DeprecatedLineIOSegmentTest, SingleVectorParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V % VectorSize(3);

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
};

TEST(DeprecatedLineIOSegmentTest, MultipleVectorsPrinting) {
    vector<int> V = {1, 2, 3};
    vector<int> W = {4, 5, 6, 7};

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3 4 5 6 7\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, MultipleVectorsParsing) {
    vector<int> V, W;

    LineIOSegment segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    istringstream sin("1 2 3 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
}

TEST(DeprecatedLineIOSegmentTest, MixedVariablesPrinting) {
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

TEST(DeprecatedLineIOSegmentTest, MixedVariablesParsing) {
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

TEST(DeprecatedLineIOSegmentTest, VectorWithoutSizePrinting) {
    vector<int> V = {1, 2, 3};

    LineIOSegment segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, EmptyVectorWithoutSizePrinting) {
    vector<int> V = {};

    LineIOSegment segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(DeprecatedLineIOSegmentTest, VectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(DeprecatedLineIOSegmentTest, EmptyVectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{}), V);
}

TEST(DeprecatedLineIOSegmentTest, MixedWithVectorWithoutSizePrinting) {
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

TEST(DeprecatedLineIOSegmentTest, MixedWithVectorWithoutSizeParsing) {
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

TEST(DeprecatedLineIOSegmentTest, VectorWithoutSizeNoElementsParsing) {
    vector<int> V;

    LineIOSegment segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_TRUE(V.empty());
}

TEST(DeprecatedLineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfSpacePrefix) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfStrangeCharBetweenElements) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfNoNewline) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseOfTrailingSpace) {
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

TEST(DeprecatedLineIOSegmentTest, FailedParsingVectorWithoutSizeBecauseItIsNotLast) {
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
