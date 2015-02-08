#include "gtest/gtest.h"

#include "tcframe/io.h"

#include <sstream>

using std::ostringstream;
using tcframe::IOFormat;
using tcframe::IOFormatsCollector;
using tcframe::IOMode;
using tcframe::LineIOSegment;

TEST(LineIOSegmentTest, EmptyLinePrinting) {
    LineIOSegment segment;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(LineIOSegmentTest, SingleVariablePrinting) {
    int X;

    LineIOSegment segment;
    segment % X;

    X = 42;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleVariablesPrinting) {
    int A, B, C;

    LineIOSegment segment;
    segment % A, B, C;

    A = 42;
    B = 7;
    C = 123;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42 7 123\n", sout.str());
}

TEST(IOFormatTest, MultipleLinesPrinting) {
    int A, B;
    int K;

    IOFormat format;

    format.addSegment(&((*(new LineIOSegment())) % A, B));
    format.addSegment(&((*(new LineIOSegment())) % K));

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

    collector.line() % A, B;
    collector.line() % K;

    A = 1;
    B = 2;
    K = 77;

    IOFormat* format = collector.collectFormat(IOMode::INPUT);

    ostringstream sout;
    format->printTo(sout);

    EXPECT_EQ("1 2\n77\n", sout.str());
}
