#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/spec/io/RawLineIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class RawLineIOSegmentManipulatorTests : public Test {
protected:
    string S;

    RawLineIOSegment* segment = RawLineIOSegmentBuilder()
            .addScalarVariable(Scalar::createRaw(S, "S"))
            .build();
    
    RawLineIOSegmentManipulator manipulator;
};

TEST_F(RawLineIOSegmentManipulatorTests, Parsing_Successful) {
    istringstream in("  123 45 \n");

    manipulator.parse(segment, &in);
    EXPECT_THAT(S, Eq("  123 45 "));
}

TEST_F(RawLineIOSegmentManipulatorTests, Parsing_Successful_CheckLastVariable) {
    istringstream in("  123 45 \n");

    EXPECT_THAT(manipulator.parse(segment, &in), Eq("'S'"));
}

TEST_F(RawLineIOSegmentManipulatorTests, Parsing_Failed_MissingNewline) {
    istringstream in("  123 45 ");

    try {
        manipulator.parse(segment, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'S'"));
    }
}

TEST_F(RawLineIOSegmentManipulatorTests, Printing_Successful) {
    ostringstream out;

    S = "  123 45 ";

    manipulator.print(segment, &out);
    EXPECT_THAT(out.str(), Eq("  123 45 \n"));
}

}
