#include "gmock/gmock.h"

#include "FakeVariable.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class VariableTests : public Test {
protected:
    int X;
    FakeVariable var = FakeVariable("X", VariableType::SCALAR);
};


TEST_F(VariableTests, SuccessfulParsing) {
    istream* in = new istringstream("123");
    var.parseValue(in, X, "'X'");
    EXPECT_THAT(X, Eq(123));
}

TEST_F(VariableTests, FailedParsingBecauseWhitespace) {
    istream* in = new istringstream(" 123");
    try {
        var.parseValue(in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: <whitespace>"));
    }
}

TEST_F(VariableTests, FailedParsingBecauseEof) {
    istream* in = new istringstream("");
    try {
        var.parseValue(in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: <EOF>"));
    }
}

TEST_F(VariableTests, FailedParsingBecauseOverflow) {
    istream* in = new istringstream("12345678901234567890");
    try {
        var.parseValue(in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: '12345678901234567890'"));
    }
}

TEST_F(VariableTests, FailedParsingBecauseTypeMismatch) {
    istream* in = new istringstream("abc123");
    try {
        var.parseValue(in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: 'abc123'"));
    }
}

}
