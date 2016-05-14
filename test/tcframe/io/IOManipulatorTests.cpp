#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "tcframe/io/IOManipulator.hpp"

using ::testing::Eq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class IOManipulatorTests : public Test {
protected:
    int A;
    int B;

    IOSegment* segmentA = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(A, "A"))
            .build();
    IOSegment* segmentB = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(B, "B"))
            .build();
    IOFormat ioFormat = IOFormatBuilder()
            .prepareForInputFormat()
            .addIOSegment(segmentA)
            .addIOSegment(segmentB)
            .build();

    IOManipulator manipulator = IOManipulator(ioFormat);
};

TEST_F(IOManipulatorTests, Parsing_Successful) {
    istringstream in("123\n42\n");
    manipulator.parseInput(&in);
    EXPECT_THAT(A, Eq(123));
    EXPECT_THAT(B, Eq(42));
}

TEST_F(IOManipulatorTests, Parsing_FailedBecauseNoEof) {
    istringstream in("123\n42\nbogus");
    EXPECT_THROW({manipulator.parseInput(&in);}, runtime_error);
}

TEST_F(IOManipulatorTests, Printing_Successful) {
    A = 123;
    B = 42;
    ostringstream out;
    manipulator.printInput(&out);
    EXPECT_THAT(out.str(), Eq("123\n42\n"));
}

}
