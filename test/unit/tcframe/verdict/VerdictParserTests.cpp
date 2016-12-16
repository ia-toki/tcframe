#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/verdict/VerdictParser.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class VerdictParserTests : public Test {};

TEST_F(VerdictParserTests, Successful_AC) {
    EXPECT_THAT(VerdictParser::parse(new istringstream("AC\n")), Eq(Verdict::ac()));
}

TEST_F(VerdictParserTests, Successful_WA) {
    EXPECT_THAT(VerdictParser::parse(new istringstream("WA\n")), Eq(Verdict::wa()));
}

TEST_F(VerdictParserTests, Failed_Empty) {
    try {
        VerdictParser::parse(new istringstream(""));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <verdict> on the first line"));
    }
}

TEST_F(VerdictParserTests, Failed_UnknownVerdict) {
    try {
        VerdictParser::parse(new istringstream("hokus pokus"));
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Unknown verdict: hokus pokus"));
    }
}

}
