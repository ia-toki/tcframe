#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../variable/MockScalar.hpp"
#include "MockWhitespaceManipulator.hpp"
#include "tcframe/io/LineIOSegmentManipulator.hpp"

using ::testing::InSequence;
using ::testing::Test;

using std::ostringstream;

namespace tcframe {

class LineIOSegmentManipulatorTests : public Test {
protected:
    Mock(Scalar) scalarA;
    Mock(Scalar) scalarB;
    Mock(Scalar) scalarC;
    Mock(WhitespaceManipulator) whitespaceManipulator;
    ostream* out = new ostringstream();

    LineIOSegmentManipulator manipulator = LineIOSegmentManipulator(&whitespaceManipulator);
};

TEST_F(LineIOSegmentManipulatorTests, ScalarsPrinting) {
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(&scalarA)
            .addScalarVariable(&scalarB)
            .addScalarVariable(&scalarC)
            .build();

    {
        InSequence sequence;
        EXPECT_CALL(scalarA, printTo(out));
        EXPECT_CALL(whitespaceManipulator, printSpace(out))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarB, printTo(out));
        EXPECT_CALL(whitespaceManipulator, printSpace(out))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarC, printTo(out));
        EXPECT_CALL(whitespaceManipulator, printNewline(out));
    }

    manipulator.print(segment, out);
}

}
