#include "gtest/gtest.h"

#include "tcframe/iovariable/VerticalVector.hpp"
#include "tcframe/iovariable/ParsingException.hpp"

#include <sstream>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::vector;

using tcframe::ParsingException;
using tcframe::VerticalVector;

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
