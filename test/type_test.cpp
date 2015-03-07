#include "gtest/gtest.h"

#include "tcframe/type.hpp"

#include <sstream>

using std::ostringstream;
using tcframe::HorizontalVector;
using tcframe::Matrix;
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
