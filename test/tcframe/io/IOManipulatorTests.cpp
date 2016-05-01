#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../io/FakeIOSegment.hpp"
#include "../io/MockLineIOSegmentManipulator.hpp"
#include "tcframe/io/IOManipulator.hpp"

using ::testing::InSequence;
using ::testing::Test;

using std::ostringstream;

namespace tcframe {

class IOManipulatorTests : public Test {
protected:
    Mock(LineIOSegmentManipulator) lineIOSegmentManipulator;
    IOSegment* segmentA = new FakeIOSegment(IOSegmentType::LINE);
    IOSegment* segmentB = new FakeIOSegment(IOSegmentType::LINE);
    IOFormat ioFormat = IOFormatBuilder()
            .prepareForInputFormat()
            .addIOSegment(segmentA)
            .addIOSegment(segmentB)
            .build();

    ostream* out = new ostringstream();

    IOManipulator manipulator = IOManipulator(&lineIOSegmentManipulator, ioFormat);
};

TEST_F(IOManipulatorTests, Printing) {
    {
        InSequence sequence;

        EXPECT_CALL(lineIOSegmentManipulator, print(reinterpret_cast<LineIOSegment*>(segmentA), out));
        EXPECT_CALL(lineIOSegmentManipulator, print(reinterpret_cast<LineIOSegment*>(segmentB), out));
    }

    manipulator.printInput(out);
}

}
