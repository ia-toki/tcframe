#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../io/FakeIOSegment.hpp"
#include "../io/MockLineIOSegmentManipulator.hpp"
#include "../io/MockWhitespaceManipulator.hpp"
#include "tcframe/io/IOManipulator.hpp"

using ::testing::InSequence;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class IOManipulatorTests : public Test {
protected:
    Mock(LineIOSegmentManipulator) lineIOSegmentManipulator;
    Mock(WhitespaceManipulator) whitespaceManipulator;
    IOSegment* segmentA = new FakeIOSegment(IOSegmentType::LINE);
    IOSegment* segmentB = new FakeIOSegment(IOSegmentType::LINE);
    IOFormat ioFormat = IOFormatBuilder()
            .prepareForInputFormat()
            .addIOSegment(segmentA)
            .addIOSegment(segmentB)
            .build();

    istream* in = new istringstream();
    ostream* out = new ostringstream();

    IOManipulator manipulator = IOManipulator(&lineIOSegmentManipulator, &whitespaceManipulator, ioFormat);
};

TEST_F(IOManipulatorTests, Parsing) {
    {
        InSequence sequence;

        EXPECT_CALL(lineIOSegmentManipulator, parse(reinterpret_cast<LineIOSegment*>(segmentA), in));
        EXPECT_CALL(lineIOSegmentManipulator, parse(reinterpret_cast<LineIOSegment*>(segmentB), in));
        EXPECT_CALL(whitespaceManipulator, ensureEof(in));
    }

    manipulator.parseInput(in);
}

TEST_F(IOManipulatorTests, Printing) {
    {
        InSequence sequence;

        EXPECT_CALL(lineIOSegmentManipulator, print(reinterpret_cast<LineIOSegment*>(segmentA), out));
        EXPECT_CALL(lineIOSegmentManipulator, print(reinterpret_cast<LineIOSegment*>(segmentB), out));
    }

    manipulator.printInput(out);
}

}
