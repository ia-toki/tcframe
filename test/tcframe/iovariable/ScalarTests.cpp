#include "gtest/gtest.h"

#include "tcframe/iovariable/Scalar.hpp"
#include "tcframe/iovariable/ParsingException.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using tcframe::ParsingException;
using tcframe::Scalar;

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
