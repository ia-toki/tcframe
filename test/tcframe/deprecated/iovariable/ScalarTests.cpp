#include "gtest/gtest.h"

#include "tcframe/deprecated/iovariable/Scalar.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using tcframe::ParsingException;
using tcframe::Scalar;

TEST(DeprecatedScalarTest, Construction) {
    int X;
    Scalar<int> wrappedX(X, "X");

    EXPECT_EQ("X", wrappedX.getName());
}


TEST(DeprecatedScalarTest, Printing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    X = 42;
    ostringstream sout;
    wrappedX.printTo(sout);

    EXPECT_EQ("42", sout.str());
}

TEST(DeprecatedScalarTest, FailedParsingBecauseWhitespace) {
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

TEST(DeprecatedScalarTest, FailedParsingBecauseOverflow) {
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

TEST(DeprecatedScalarTest, FailedParsingBecauseTypeMismatch) {
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

TEST(DeprecatedScalarTest, FailedParsingBecauseEof) {
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

TEST(DeprecatedScalarTest, Parsing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    istringstream sin("123");
    wrappedX.parseFrom(sin);

    EXPECT_EQ(123, X);
}

TEST(DeprecatedScalarTest, Clearing) {
    int X;
    Scalar<int> wrappedX(X, "X");

    X = 42;
    wrappedX.clear();

    EXPECT_EQ(0, X);
}
