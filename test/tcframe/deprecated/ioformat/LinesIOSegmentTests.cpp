#include "gtest/gtest.h"

#include "tcframe/deprecated/ioformat/IOFormatException.hpp"
#include "tcframe/deprecated/ioformat/LinesIOSegment.hpp"
#include "tcframe/deprecated/ioformat/VectorSize.hpp"
#include "tcframe/deprecated/ioformat/VectorWithSize.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"

#include <sstream>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::vector;

using tcframe_old::IOFormatException;
using tcframe_old::LinesIOSegmentOld;
using tcframe_old::ParsingException;
using tcframe_old::PrintingException;
using tcframe_old::VectorSize;
using tcframe_old::VectorWithSize;

TEST(DeprecatedLinesIOSegmentOldTest, UnsupportedTypes) {
    int X;

    LinesIOSegmentOld segment("X");

    try {
        segment, X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Variable type of `X` unsatisfied. Expected: (jagged) vector of basic scalar or string type", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, NoVariables) {
    LinesIOSegmentOld segment("");
    segment % VectorSize(4);

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Lines segment must have at least one variable", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, NoVectorSizes) {
    vector<int> V;

    LinesIOSegmentOld segment("V");
    segment, V;

    try {
        segment.checkState();
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Lines segment must define vector sizes", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedPrintingBecauseVectorSizesMismatch) {
    int N = 3;
    vector<int> V = vector<int>{1, 2, 3};
    vector<int> W = vector<int>{4, 5, 6, 7};

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(N);

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (PrintingException& e) {
        EXPECT_EQ("Number of elements of vector `W` unsatisfied. Expected: 3, actual: 4", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingBecauseNoSpace) {
    vector<int> V, W;

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 2\n3\n5 6\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> after variable `V[1]`", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingBecauseNoNewLine) {
    vector<int> V, W;

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 2\n3 4\n5 6");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <new line> after variable `W[2]`", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, SingleVectorPrinting) {
    vector<int> V = {1, 2, 3};

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1\n2\n3\n", sout.str());
}

TEST(DeprecatedLinesIOSegmentOldTest, SingleVectorParsing) {
    vector<int> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1\n2\n3\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(DeprecatedLinesIOSegmentOldTest, MultipleVectorsPrinting) {
    vector<int> V = {1, 2, 3};
    vector<string> W = {"a", "bb", "ccc"};

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 a\n2 bb\n3 ccc\n", sout.str());
}

TEST(DeprecatedLinesIOSegmentOldTest, MultipleVectorsParsing) {
    vector<int> V;
    vector<string> W;

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 a\n2 bb\n3 ccc\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<string>{"a", "bb", "ccc"}), W);
}

TEST(DeprecatedLinesIOSegmentOldTest, JaggedVectorPrinting) {
    vector<vector<int>> V = {{1}, {2, 3}, {4, 5, 6}};

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1\n2 3\n4 5 6\n", sout.str());
}

TEST(DeprecatedLinesIOSegmentOldTest, JaggedVectorParsing) {
    vector<vector<int>> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1\n2 3\n4 5 6\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<vector<int>>{{1}, {2, 3}, {4, 5, 6}}), V);
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingJaggedMatrixBecauseOfSpacePrefix) {
    vector<vector<int>> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(3);

    istringstream sin("1 2 3\n 4 5 6\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[1][0]`. Found: <whitespace>", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingJaggedVectorBecauseOfStrangeCharBetweenElements) {
    vector<vector<int>> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2\t3\n");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[0][1]`", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingJaggedVectorBecauseOfNoNewline) {
    vector<vector<int>> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2 3\n4 5 6");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Expected: <space> or <new line> after variable `V[1][2]`", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingJaggedVectorBecauseOfTrailingSpace) {
    vector<vector<int>> V;

    LinesIOSegmentOld segment("V");
    (segment, V) % VectorSize(2);

    istringstream sin("1 2 3\n4 5 6 ");

    try {
        segment.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[1][3]`. Found: <EOF>", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, FailedParsingJaggedVectorBecauseItIsNotLast) {
    vector<vector<int>> V;
    vector<int> W;

    LinesIOSegmentOld segment("V, W");

    try {
        (segment, V, W) % VectorSize(4);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_EQ("Jagged vector can only be the last variable in a lines segment", e.getMessage());
    }
}

TEST(DeprecatedLinesIOSegmentOldTest, MixedWithJaggedVectorPrinting) {
    vector<int> V = vector<int>{1, 2, 3};
    vector<vector<int>> W = vector<vector<int>>{{4}, {5, 6}, {7, 8, 9}};

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 4\n2 5 6\n3 7 8 9\n", sout.str());
}

TEST(DeprecatedLinesIOSegmentOldTest, MixedWithJaggedVectorSizeParsing) {
    vector<int> V;
    vector<vector<int>> W;

    LinesIOSegmentOld segment("V, W");
    (segment, V, W) % VectorSize(3);

    istringstream sin("1 4\n2 5 6\n3 7 8 9\n");
    segment.parseFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
    EXPECT_EQ((vector<vector<int>>{{4}, {5, 6}, {7, 8, 9}}), W);
}
