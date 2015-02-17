#include "gtest/gtest.h"

#include "tcframe/type.h"

#include <sstream>

using std::ostringstream;
using tcframe::Matrix;
using tcframe::Scalar;

TEST(ScalarTest, Printing) {
    int x;
    Scalar<int> wrappedX(x);

    x = 42;
    ostringstream sout;
    wrappedX.printTo(sout);

    EXPECT_EQ("42", sout.str());
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
