#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/util/WhitespaceUtils.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;
using std::runtime_error;

namespace tcframe {

class WhitespaceUtilsTests : public Test {};

TEST_F(WhitespaceUtilsTests, Parsing_Space_Successful) {
    istringstream in(" 123");
    EXPECT_NO_THROW({
        WhitespaceUtils::parseSpace(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceUtilsTests, Parsing_Space_Failed) {
    istringstream in("123");
    try {
        WhitespaceUtils::parseSpace(&in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'N'"));
    }
}

TEST_F(WhitespaceUtilsTests, Parsing_SpaceAfterMissingNewline_Successful) {
    istringstream in(" 123");
    EXPECT_NO_THROW({
        WhitespaceUtils::parseSpaceAfterMissingNewline(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceUtilsTests, Parsing_SpaceAfterMissingNewline_Failed) {
    istringstream in("123");
    try {
        WhitespaceUtils::parseSpaceAfterMissingNewline(&in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> or <newline> after 'N'"));
    }
}

TEST_F(WhitespaceUtilsTests, Parsing_Newline_Successful) {
    istringstream in("\n123");
    EXPECT_NO_THROW({
        WhitespaceUtils::parseNewline(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceUtilsTests, Parsing_Newline_Failed) {
    istream* in = new istringstream("123");
    try {
        WhitespaceUtils::parseNewline(in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'N'"));
    }
}

TEST_F(WhitespaceUtilsTests, EnsuringEof_Successful) {
    istream* in = new istringstream("");
    EXPECT_NO_THROW({
        WhitespaceUtils::ensureEof(in);
    });
}

TEST_F(WhitespaceUtilsTests, EnsuringEof_Failed) {
    istream* in = new istringstream("123");
    try {
        WhitespaceUtils::ensureEof(in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <EOF>"));
    }
}

}
