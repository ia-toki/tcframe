#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/spec/variable/WhitespaceManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;
using std::runtime_error;

namespace tcframe {

class WhitespaceManipulatorTests : public Test {};

TEST_F(WhitespaceManipulatorTests, Parsing_Space_Successful) {
    istringstream in(" 123");
    EXPECT_NO_THROW({
        WhitespaceManipulator::parseSpace(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceManipulatorTests, Parsing_Space_Failed) {
    istringstream in("123");
    try {
        WhitespaceManipulator::parseSpace(&in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'N'"));
    }
}

TEST_F(WhitespaceManipulatorTests, Parsing_SpaceAfterMissingNewline_Successful) {
    istringstream in(" 123");
    EXPECT_NO_THROW({
        WhitespaceManipulator::parseSpaceAfterMissingNewline(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceManipulatorTests, Parsing_SpaceAfterMissingNewline_Failed) {
    istringstream in("123");
    try {
        WhitespaceManipulator::parseSpaceAfterMissingNewline(&in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> or <newline> after 'N'"));
    }
}

TEST_F(WhitespaceManipulatorTests, Parsing_Newline_Successful) {
    istringstream in("\n123");
    EXPECT_NO_THROW({
        WhitespaceManipulator::parseNewline(&in, "'N'");
    });
    int M;
    in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceManipulatorTests, Parsing_Newline_Failed) {
    istream* in = new istringstream("123");
    try {
        WhitespaceManipulator::parseNewline(in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'N'"));
    }
}

TEST_F(WhitespaceManipulatorTests, IsEof_True) {
    istream* in = new istringstream("");
    EXPECT_TRUE(WhitespaceManipulator::isEof(in));
}

TEST_F(WhitespaceManipulatorTests, IsEof_False) {
    istream* in = new istringstream("123");
    EXPECT_FALSE(WhitespaceManipulator::isEof(in));
}

TEST_F(WhitespaceManipulatorTests, EnsuringEof_Successful) {
    istream* in = new istringstream("");
    EXPECT_NO_THROW({
        WhitespaceManipulator::ensureEof(in);
    });
}

TEST_F(WhitespaceManipulatorTests, EnsuringEof_Failed) {
    istream* in = new istringstream("123");
    try {
        WhitespaceManipulator::ensureEof(in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <EOF>"));
    }
    try {
        WhitespaceManipulator::ensureEof(in, "'N'");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <EOF> after 'N'"));
    }
}

}
