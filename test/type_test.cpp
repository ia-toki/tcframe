#include "gtest/gtest.h"

#include "tcframe/type.h"

#include <sstream>

using std::ostringstream;
using tcframe::HorizontalVector;
using tcframe::Matrix;
using tcframe::Scalar;
using tcframe::VerticalVector;

TEST(ScalarTest, Printing) {
    int x;
    Scalar<int> wrappedX(x);

    x = 42;
    ostringstream sout;
    wrappedX.printTo(sout);

    EXPECT_EQ("42", sout.str());
}

TEST(HorizontalVectorTest, Printing) {
    vector<int> V;
    HorizontalVector<int> wrappedV(V);

    V = vector<int>{1, 2, 3};
    ostringstream sout;
    wrappedV.printTo(sout);

    EXPECT_EQ("1 2 3", sout.str());
}

TEST(VerticalVectorTest, Printing) {
    vector<int> V;
    VerticalVector<int> wrappedV(V);

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

TEST(MatrixTest, CharPrinting) {
    vector<vector<char>> C;
    Matrix<char> wrappedC(C);

    C = vector<vector<char>>{ {'a', 'b'}, {'c', 'd'} };

    ostringstream sout;
    wrappedC.printTo(sout);

    EXPECT_EQ("ab\ncd\n", sout.str());
}

TEST(MatrixTest, NonCharPrinting) {
    vector<vector<int>> V;
    Matrix<int> wrappedV(V);

    V = vector<vector<int>>{ {1, 2}, {3, 4} };

    ostringstream sout;
    wrappedV.printTo(sout);

    EXPECT_EQ("1 2\n3 4\n", sout.str());
}
