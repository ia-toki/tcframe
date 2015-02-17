#include "gtest/gtest.h"

#include "tcframe/io.h"

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
using tcframe::TypeException;

TEST(LineIOSegmentTest, UnsupportedTypes) {
    set<int> s;

    LineIOSegment segment;

    try {
        segment % s;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("is only supported") != string::npos);
    }
}

TEST(LineIOSegmentTest, InvalidSyntaxes) {
    int A, B;

    LineIOSegment segment;

    try {
        segment, A;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("'%`") != string::npos);
    }

    try {
        segment % A % B;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("',`") != string::npos);
    }
}

TEST(LineIOSegmentTest, EmptyLinePrinting) {
    LineIOSegment segment;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("\n", sout.str());
}

TEST(LineIOSegmentTest, SingleScalarPrinting) {
    int X;

    LineIOSegment segment;
    segment % X;

    X = 42;

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("42\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleScalarsPrinting) {
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

TEST(LineIOSegmentTest, SingleVectorPrinting) {
    vector<int> V;

    LineIOSegment segment;
    segment % V;

    V = vector<int>{1, 2, 3};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3\n", sout.str());
}

TEST(LineIOSegmentTest, MultipleVectorsPrinting) {
    vector<int> V, W;

    LineIOSegment segment;
    segment % V, W;

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2 3 4 5 6\n", sout.str());
}

TEST(LineIOSegmentTest, MixedVariablesPrinting) {
    vector<int> V, W;
    int A, B;

    LineIOSegment segment;
    segment % A, V, B, W;

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    A = V.size();
    B = W.size();

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("3 1 2 3 4 4 5 6 7\n", sout.str());
}

TEST(LinesIOSegmentTest, UnsupportedTypes) {
    int X;

    LinesIOSegment segment;

    try {
        segment % X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("is only supported for vector") != string::npos);
    }

    vector<vector<int>> V;

    try {
        segment % V;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("is only supported for vector of basic scalars") != string::npos);
    }
}

TEST(LinesIOSegmentTest, IncompatibleVectorSizes) {
    vector<int> V, W;

    LinesIOSegment segment;
    segment % V, W;

    V = vector<int>{1, 2, 3};
    W = vector<int>{4, 5, 6, 7};

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("must have equal sizes") != string::npos);
    }
}

TEST(LinesIOSegmentTest, InvalidSyntaxes) {
    vector<int> V, W;

    LinesIOSegment segment;

    try {
        segment, V;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("'%`") != string::npos);
    }

    try {
        segment % V % W;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("',`") != string::npos);
    }
}

TEST(LinesIOSegmentTest, NoVariables) {
    LinesIOSegment segment;

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("must have at least one variable"));
    }
}

TEST(LinesIOSegmentTest, SingleVectorPrinting) {
    vector<int> V;

    LinesIOSegment segment;
    segment % V;

    V = vector<int>{1, 2, 3};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1\n2\n3\n", sout.str());
}

TEST(LinesIOSegmentTest, MultipleVectorsPrinting) {
    vector<int> V;
    vector<string> W;

    LinesIOSegment segment;
    segment % V, W;

    V = vector<int>{1, 2, 3};
    W = vector<string>{"a", "bb", "ccc"};

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 a\n2 bb\n3 ccc\n", sout.str());
}

TEST(GridIOSegmentTest, UnsupportedTypes) {
    int X;

    GridIOSegment segment;

    try {
        segment % X;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("is only supported for matrix") != string::npos);
    }

    vector<vector<vector<int>>> V;

    try {
        segment % V;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("is only supported for matrix of basic scalars") != string::npos);
    }
}

TEST(GridIOSegmentTest, IncompatibleDimensionSizes) {
    vector<vector<int>> V;

    GridIOSegment segment;
    segment % V;

    V = vector<vector<int>>{ {1, 2}, {3, 4, 5} };

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (TypeException& e) {
        EXPECT_TRUE(string(e.what()).find("must have equal number of columns") != string::npos);
    }
}

TEST(GridIOSegmentTest, NonSingularVariables) {
    GridIOSegment segment;

    ostringstream sout;

    try {
        segment.printTo(sout);
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("must have exactly one variable") != string::npos);
    }

    vector<vector<int>> V, W;


    try {
        segment % V % W;
        FAIL();
    } catch (IOFormatException& e) {
        EXPECT_TRUE(string(e.what()).find("must have exactly one variable") != string::npos);
    }
}

TEST(GridIOSegmentTest, CharPrinting) {
    vector<vector<char>> C;

    GridIOSegment segment;
    segment % C;

    C = vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} };

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("ab\ncd\n", sout.str());
}

TEST(GridIOSegmentTest, NonCharPrinting) {
    vector<vector<int>> C;

    GridIOSegment segment;
    segment % C;

    C = vector<vector<int>>{ {1, 2}, {3, 4} };

    ostringstream sout;
    segment.printTo(sout);

    EXPECT_EQ("1 2\n3 4\n", sout.str());
}

TEST(IOFormatTest, MultipleLinesPrinting) {
    int A, B;
    int K;
    vector<int> V;
    vector<int> W;
    vector<string> Z;
    vector<vector<char>> C;
    vector<vector<int>> P;

    IOFormat format;

    format.addSegment(&((*(new LineIOSegment())) % A, B));
    format.addSegment(&((*(new LineIOSegment())) % K));
    format.addSegment(&((*(new LineIOSegment())) % V));
    format.addSegment(&((*(new LinesIOSegment())) % W, Z));
    format.addSegment(&((*(new GridIOSegment())) % C));
    format.addSegment(&((*(new GridIOSegment())) % P));

    A = 1;
    B = 2;
    K = 77;

    V = vector<int>{11, 22, 33};

    W = vector<int>{10, 20};
    Z = vector<string>{"x", "y"};

    C = vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} };
    P = vector<vector<int>>{ {1, 2}, {3, 4} };

    ostringstream sout;
    format.printTo(sout);

    EXPECT_EQ("1 2\n77\n11 22 33\n10 x\n20 y\nab\ncd\n1 2\n3 4\n", sout.str());
}

TEST(IOFormatsCollectorTest, InputFormatCollection) {
    int A, B;
    int K;
    vector<int> V;
    vector<int> W;
    vector<string> Z;
    vector<vector<char>> C;
    vector<vector<int>> P;

    IOFormatsCollector collector;

    collector.line() % A, B;
    collector.line() % K;
    collector.line() % V;
    collector.lines() % W, Z;
    collector.grid() % C;
    collector.grid() % P;

    A = 1;
    B = 2;
    K = 77;

    V = vector<int>{11, 22, 33};

    W = vector<int>{10, 20};
    Z = vector<string>{"x", "y"};

    C = vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} };
    P = vector<vector<int>>{ {1, 2}, {3, 4} };

    IOFormat* format = collector.collectFormat(IOMode::INPUT);

    ostringstream sout;
    format->printTo(sout);

    EXPECT_EQ("1 2\n77\n11 22 33\n10 x\n20 y\nab\ncd\n1 2\n3 4\n", sout.str());
}
