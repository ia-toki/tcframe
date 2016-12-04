#include "gmock/gmock.h"

#include "tcframe/spec/variable/Variable.hpp"

#include <sstream>

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class VariableTests : public Test {
protected:
    int X;
    string S;
};

TEST_F(VariableTests, Parsing_Successful) {
    istringstream in("123");
    Variable::parseValue(&in, X, "'X'");
    EXPECT_THAT(X, Eq(123));
}

TEST_F(VariableTests, Parsing_Failed_FoundWhitespace) {
    istringstream in(" 123");
    try {
        Variable::parseValue(&in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: <whitespace>"));
    }
}

TEST_F(VariableTests, Parsing_Failed_FoundEof) {
    istringstream in("");
    try {
        Variable::parseValue(&in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: <EOF>"));
    }
}

TEST_F(VariableTests, Parsing_Failed_Overflow) {
    istringstream in("12345678901234567890");
    try {
        Variable::parseValue(&in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: '12345678901234567890'"));
    }
}

TEST_F(VariableTests, Parsing_Failed_TypeMismatch) {
    istringstream in("abc123");
    try {
        Variable::parseValue(&in, X, "'X'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'X'. Found: 'abc123'"));
    }
}

TEST_F(VariableTests, Parsing_Raw_Successful) {
    istringstream in("  123 45 \nabc");
    Variable::parseRawLine(&in, S);
    EXPECT_THAT(S, Eq("  123 45 "));
}

TEST_F(VariableTests, Parsing_Raw_Successful_Empty) {
    istringstream in("\nabc");
    Variable::parseRawLine(&in, S);
    EXPECT_THAT(S, Eq(""));
}

TEST_F(VariableTests, Parsing_Raw_Successful_Eof) {
    istringstream in("  123 45 ");
    Variable::parseRawLine(&in, S);
    EXPECT_THAT(S, Eq("  123 45 "));
}


}
