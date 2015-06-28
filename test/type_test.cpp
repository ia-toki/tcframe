#include "gtest/gtest.h"

#include "tcframe/type.hpp"

using tcframe::HorizontalVector;
using tcframe::Matrix;
using tcframe::ParsingException;
using tcframe::PrintingException;
using tcframe::Scalar;
using tcframe::VerticalVector;

TEST(ScalarTest, Construction) {
    int X;
    Scalar<int> wrappedX(X, "X");

    EXPECT_EQ("X", wrappedX.getName());
}

TEST(ScalarTest, Printing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    X = 42;
    ostringstream sout;
    wrappedX.printTo(sout);

    EXPECT_EQ("42", sout.str());
}

TEST(ScalarTest, FailedParsingBecauseWhitespace) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin(" 123");

    try {
        wrappedX.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `X`. Found: <whitespace>", e.getMessage());
    }
}

TEST(ScalarTest, FailedParsingBecauseOverflow) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin("12345678901234567890");

    try {
        wrappedX.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `X`. Found: '12345678901234567890'", e.getMessage());
    }
}

TEST(ScalarTest, FailedParsingBecauseTypeMismatch) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin("abc123");

    try {
        wrappedX.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `X`. Found: 'abc123'", e.getMessage());
    }
}

TEST(ScalarTest, FailedParsingBecauseEof) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin("");

    try {
        wrappedX.parseFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `X`. Found: <EOF>", e.getMessage());
    }
}

TEST(ScalarTest, Parsing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin("123");
    wrappedX.parseFrom(sin);

    EXPECT_EQ(123, X);
}

TEST(ScalarTest, Clearing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    X = 42;
    wrappedX.clear();

    EXPECT_EQ(0, X);
}

TEST(HorizontalVectorTest, Construction) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};

    EXPECT_EQ("V", wrappedV.getName());
    EXPECT_EQ(3, wrappedV.size());
}

TEST(HorizontalVectorTest, Printing) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};
    ostringstream sout;
    wrappedV.printElementTo(0, sout);
    sout << " ";
    wrappedV.printElementTo(1, sout);
    sout << " ";
    wrappedV.printElementTo(2, sout);

    EXPECT_EQ("1 2 3", sout.str());
}

TEST(HorizontalVectorTest, FailedParsingAndAdding) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V, "V");

    istringstream sin("1 a 3");

    try {
        wrappedV.parseAndAddElementFrom(sin);
        sin.get();
        wrappedV.parseAndAddElementFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[1]`. Found: 'a'", e.getMessage());
    }
}

TEST(HorizontalVectorTest, ParsingAndAdding) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V, "V");

    istringstream sin("1 2 3");

    wrappedV.parseAndAddElementFrom(sin);
    sin.get();
    wrappedV.parseAndAddElementFrom(sin);
    sin.get();
    wrappedV.parseAndAddElementFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(HorizontalVectorTest, Clearing) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};
    wrappedV.clear();

    EXPECT_EQ(vector<int>(), V);
}

TEST(VerticalVectorTest, Construction) {
    vector<int> V;
    VerticalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};

    EXPECT_EQ("V", wrappedV.getName());
    EXPECT_EQ(3, wrappedV.size());
}

TEST(VerticalVectorTest, Printing) {
    vector<int> V;
    VerticalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};
    ostringstream sout;
    wrappedV.printElementTo(0, sout);
    sout << "\n";
    wrappedV.printElementTo(1, sout);
    sout << "\n";
    wrappedV.printElementTo(2, sout);
    sout << "\n";

    EXPECT_EQ("1\n2\n3\n", sout.str());
}

TEST(VerticalVectorTest, FailedParsingAndAdding) {
    vector<int> V;
    VerticalVector<int> wrappedV(V, "V");

    istringstream sin("1 2 a");

    try {
        wrappedV.parseAndAddElementFrom(sin);
        sin.get();
        wrappedV.parseAndAddElementFrom(sin);
        sin.get();
        wrappedV.parseAndAddElementFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `V[2]`. Found: 'a'", e.getMessage());
    }
}

TEST(VerticalVectorTest, ParsingAndAdding) {
    vector<int> V;
    VerticalVector<int> wrappedV(V, "V");

    istringstream sin("1 2 3");

    wrappedV.parseAndAddElementFrom(sin);
    sin.get();
    wrappedV.parseAndAddElementFrom(sin);
    sin.get();
    wrappedV.parseAndAddElementFrom(sin);

    EXPECT_EQ((vector<int>{1, 2, 3}), V);
}

TEST(VerticalVectorTest, Clearing) {
    vector<int> V;
    VerticalVector<int> wrappedV(V, "V");

    V = vector<int>{1, 2, 3};
    wrappedV.clear();

    EXPECT_EQ(vector<int>(), V);
}

TEST(MatrixTest, Construction) {
    vector<vector<int>> G;
    Matrix<int> wrappedG(G, "G");

    G = vector<vector<int>>{ {1, 2}, {3, 4, 5} };

    EXPECT_EQ("G", wrappedG.getName());
    ASSERT_EQ(2, wrappedG.rowsSize());
    EXPECT_EQ(2, wrappedG.columnsSize(0));
    EXPECT_EQ(3, wrappedG.columnsSize(1));
}

TEST(MatrixTest, Printing) {
    vector<vector<int>> G;
    Matrix<int> wrappedG(G, "G");

    G = vector<vector<int>>{ {1, 2}, {3, 4} };

    ostringstream sout;
    wrappedG.printElementTo(0, 0, sout);
    sout << " ";
    wrappedG.printElementTo(0, 1, sout);
    sout << "\n";
    wrappedG.printElementTo(1, 0, sout);
    sout << " ";
    wrappedG.printElementTo(1, 1, sout);
    sout << "\n";

    EXPECT_EQ("1 2\n3 4\n", sout.str());
}

TEST(MatrixTest, FailedParsingAndAdding) {
    vector<vector<int>> G;
    Matrix<int> wrappedG(G, "G");

    istringstream sin("1 2\na 4\n");

    try {
        wrappedG.addRow();
        wrappedG.parseAndAddColumnElementFrom(sin);
        sin.get();
        wrappedG.parseAndAddColumnElementFrom(sin);
        sin.get();
        wrappedG.addRow();
        wrappedG.parseAndAddColumnElementFrom(sin);
        FAIL();
    } catch (ParsingException& e) {
        EXPECT_EQ("Cannot parse for variable `G[1][0]`. Found: 'a'", e.getMessage());
    }

}

TEST(MatrixTest, ParsingAndAdding) {
    vector<vector<int>> G;
    Matrix<int> wrappedG(G, "G");

    istringstream sin("1 2\n3 4\n");

    wrappedG.addRow();
    wrappedG.parseAndAddColumnElementFrom(sin);
    sin.get();
    wrappedG.parseAndAddColumnElementFrom(sin);
    sin.get();
    wrappedG.addRow();
    wrappedG.parseAndAddColumnElementFrom(sin);
    sin.get();
    wrappedG.parseAndAddColumnElementFrom(sin);
    sin.get();

    EXPECT_EQ((vector<vector<int>>{ {1, 2}, {3, 4} }), G);
}

TEST(MatrixTest, Clearing) {
    vector<vector<int>> G;
    Matrix<int> wrappedG(G, "G");

    G = vector<vector<int>>{ {1, 2}, {3, 4} };
    wrappedG.clear();

    EXPECT_EQ(vector<vector<int>>(), G);
}
