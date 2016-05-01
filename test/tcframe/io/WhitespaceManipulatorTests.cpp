#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/io/WhitespaceManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class WhitespaceManipulatorTests : public Test {
protected:
    WhitespaceManipulator whitespaceManipulator;
};

TEST_F(WhitespaceManipulatorTests, SucessfulParsingSpace) {
    istream* in = new istringstream(" 123");
    EXPECT_NO_THROW({
        whitespaceManipulator.parseSpace(in, "N");
    });
    int M;
    *in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceManipulatorTests, FailedParsingSpace) {
    istream* in = new istringstream("123");
    try {
        whitespaceManipulator.parseSpace(in, "N");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'N'"));
    }
}

TEST_F(WhitespaceManipulatorTests, SucessfulParsingNewline) {
    istream* in = new istringstream("\n123");
    EXPECT_NO_THROW({
        whitespaceManipulator.parseNewline(in, "N");
    });
    int M;
    *in >> M;
    EXPECT_THAT(M, Eq(123));
}

TEST_F(WhitespaceManipulatorTests, FailedParsingNewline) {
    istream* in = new istringstream("123");
    try {
        whitespaceManipulator.parseNewline(in, "N");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'N'"));
    }
}

TEST_F(WhitespaceManipulatorTests, SucessfulEnsuringEof) {
    istream* in = new istringstream("");
    EXPECT_NO_THROW({
        whitespaceManipulator.ensureEof(in);
    });
}

TEST_F(WhitespaceManipulatorTests, FailedEnsuringEof) {
    istream* in = new istringstream("123");
    try {
        whitespaceManipulator.ensureEof(in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <EOF>"));
    }
}

TEST_F(WhitespaceManipulatorTests, PrintingSpace) {
    ostringstream* out = new ostringstream();
    *out << 123;
    whitespaceManipulator.printSpace(out);
    *out << 456;
    EXPECT_THAT(out->str(), Eq("123 456"));
}

TEST_F(WhitespaceManipulatorTests, PrintingNewline) {
    ostringstream* out = new ostringstream();
    *out << 123;
    whitespaceManipulator.printNewline(out);
    *out << 456;
    EXPECT_THAT(out->str(), Eq("123\n456"));
}

}
