#include "gmock/gmock.h"

#include <sstream>
#include <streambuf>

#include "tcframe/runner/logger/SimpleLoggerEngine.hpp"

using ::testing::Eq;
using ::testing::Test;

using std::ostringstream;
using std::streambuf;

namespace tcframe {

class SimpleLoggerEngineTests : public Test {
private:
    streambuf* coutBuf;

protected:
    ostringstream sout;
    SimpleLoggerEngine engine;

    void SetUp() {
        coutBuf = cout.rdbuf();
        cout.rdbuf(sout.rdbuf());
    }

    void TearDown() {
        cout.rdbuf(coutBuf);
    }
};

TEST_F(SimpleLoggerEngineTests, Heading) {
    engine.logHeading("HEADING");
    EXPECT_THAT(sout.str(), Eq("\n[ HEADING ]\n"));
}

TEST_F(SimpleLoggerEngineTests, Paragraph) {
    engine.logParagraph(2, "paragraph");
    EXPECT_THAT(sout.str(), Eq("    paragraph\n"));
}

TEST_F(SimpleLoggerEngineTests, HangingParagraph) {
    engine.logHangingParagraph(2, "paragraph");
    EXPECT_THAT(sout.str(), Eq("    paragraph"));
}

TEST_F(SimpleLoggerEngineTests, ListItem1) {
    engine.logListItem1(2, "item");
    EXPECT_THAT(sout.str(), Eq("    * item\n"));
}

TEST_F(SimpleLoggerEngineTests, ListItem2) {
    engine.logListItem2(2, "item");
    EXPECT_THAT(sout.str(), Eq("    - item\n"));
}

}
