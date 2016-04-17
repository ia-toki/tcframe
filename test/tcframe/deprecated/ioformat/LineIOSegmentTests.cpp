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
using tcframe::LineIOSegmentOld;
using tcframe::ParsingException;
using tcframe::PrintingException;
using tcframe::VectorSize;
using tcframe::VectorWithSize;

TEST(DeprecatedLineIOSegmentOldTest, UnsupportedType) {
    set<int> S;

    LineIOSegmentOld segment("S");

    try {
        segment, S;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `S` unsatisfied. Expected: basic scalar or string type, or vector of those types", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedPrintingBecauseVectorSizeMismatch) {
    int N = 4;
    vector<int> V = {1, 2, 3};

    LineIOSegmentOld segment("V");
    segment, V % VectorSize(N);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
        EXPECT_EQ("Number of elements of vector `V` unsatisfied. Expected: 4, actual: 3", e.getMessage());
    }
};

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingBecauseNoSpace) {
    int A, B;

    LineIOSegmentOld segment("A, B");
    segment, A, B;

    istringstream sin("7\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> after variable `A`", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingBecauseNoNewLine) {
    int A, B;

    LineIOSegmentOld segment("A, B");
    segment, A, B;

    istringstream sin("7 123");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <new line> after variable `B`", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, EmptyLinePrinting) {
    LineIOSegmentOld segment("");

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, SingleScalarPrinting) {
    int X = 42;

    LineIOSegmentOld segment("X");
    segment, X;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, SingleScalarParsing) {
    int X;

    LineIOSegmentOld segment("X");
    segment, X;

    istringstream sin("42\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, X);
}

TEST(DeprecatedLineIOSegmentOldTest, MultipleScalarsPrinting) {
    int A = 42;
    int B = 7;
    int C = 123;

    LineIOSegmentOld segment("A, B, C");
    segment, A, B, C;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42 7 123\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, MultipleScalarsParsing) {
    int A, B, C;

    LineIOSegmentOld segment("A, B, C");
    segment, A, B, C;

    istringstream sin("42 7 123\n");
    segment.parseFrom(sin);

    EXPECT_EQ(42, A);
    EXPECT_EQ(7, B);
    EXPECT_EQ(123, C);
}

TEST(DeprecatedLineIOSegmentOldTest, SingleVectorPrinting) {
    vector<int> V = {1, 2, 3};

    LineIOSegmentOld segment("V");
    segment, V % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
};

TEST(DeprecatedLineIOSegmentOldTest, SingleVectorParsing) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V % VectorSize(3);

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
};

TEST(DeprecatedLineIOSegmentOldTest, MultipleVectorsPrinting) {
    vector<int> V = {1, 2, 3};
    vector<int> W = {4, 5, 6, 7};

    LineIOSegmentOld segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3 4 5 6 7\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, MultipleVectorsParsing) {
    vector<int> V, W;

    LineIOSegmentOld segment("V, W");
    segment, V % VectorSize(3), W % VectorSize(4);

    istringstream sin("1 2 3 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
}

TEST(DeprecatedLineIOSegmentOldTest, MixedVariablesPrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    int A = V.size();
    int B = W.size();

    LineIOSegmentOld segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, MixedVariablesParsing) {
    vector<int> V, W;
    int A, B;

    LineIOSegmentOld segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W % VectorSize(4);

    istringstream sin("3 1 2 3 4 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
    EXPECT_EQ(3, A);
    EXPECT_EQ(4, B);
}

TEST(DeprecatedLineIOSegmentOldTest, VectorWithoutSizePrinting) {
    vector<int> V = {1, 2, 3};

    LineIOSegmentOld segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, EmptyVectorWithoutSizePrinting) {
    vector<int> V = {};

    LineIOSegmentOld segment("V");
    segment, V;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, VectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("1 2 3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(DeprecatedLineIOSegmentOldTest, EmptyVectorWithoutSizeParsing) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{}), V);
}

TEST(DeprecatedLineIOSegmentOldTest, MixedWithVectorWithoutSizePrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    int A = V.size();
    int B = W.size();

    LineIOSegmentOld segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(DeprecatedLineIOSegmentOldTest, MixedWithVectorWithoutSizeParsing) {
    vector<int> V, W;
    int A, B;

    LineIOSegmentOld segment("A, V, B, W");
    segment, A, V % VectorSize(3), B, W;

    istringstream sin("3 1 2 3 4 4 5 6 7\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<int>{4, 5, 6, 7}), W);
    EXPECT_EQ(3, A);
    EXPECT_EQ(4, B);
}

TEST(DeprecatedLineIOSegmentOldTest, VectorWithoutSizeNoElementsParsing) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("\n");
    segment.parseFrom(sin);

    EXPECT_TRUE(V.empty());
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingVectorWithoutSizeBecauseOfSpacePrefix) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin(" 1 2 3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[0]`. Found: <whitespace>", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingVectorWithoutSizeBecauseOfStrangeCharBetweenElements) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("1 2\t3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[1]`", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingVectorWithoutSizeBecauseOfNoNewline) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("1 2 3");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[2]`", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingVectorWithoutSizeBecauseOfTrailingSpace) {
    vector<int> V;

    LineIOSegmentOld segment("V");
    segment, V;

    istringstream sin("1 2 3 ");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[3]`. Found: <EOF>", e.getMessage());
    }
}

TEST(DeprecatedLineIOSegmentOldTest, FailedParsingVectorWithoutSizeBecauseItIsNotLast) {
    vector<int> V, W;
    int A, B;

    LineIOSegmentOld segment("A, V, B, W");

    try {
        segment, A, V, B, W % VectorSize(4);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Vector without size can only be the last variable in a line segment", e.getMessage());
    }
}
