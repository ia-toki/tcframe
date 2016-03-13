#include "gtest/gtest.h"

#include "tcframe/iovariable/HorizontalVector.hpp"
#include "tcframe/iovariable/ParsingException.hpp"

#include <sstream>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::vector;

using tcframe::ParsingException;
using tcframe::HorizontalVector;

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
