#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../variable/MockScalar.hpp"
#include "MockWhitespaceManipulator.hpp"
#include "tcframe/io/LineIOSegmentManipulator.hpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::ReturnRef;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class LineIOSegmentManipulatorTests : public Test {
protected:
    string A = "A";
    string B = "B";
    string C = "C";
    Mock(Scalar) scalarA;
    Mock(Scalar) scalarB;
    Mock(Scalar) scalarC;
    Mock(WhitespaceManipulator) whitespaceManipulator;

    LineIOSegmentManipulator manipulator = LineIOSegmentManipulator(&whitespaceManipulator);

    void SetUp() {
        ON_CALL(scalarA, name()).WillByDefault(ReturnRef(A));
        ON_CALL(scalarB, name()).WillByDefault(ReturnRef(B));
        ON_CALL(scalarC, name()).WillByDefault(ReturnRef(C));
    }
};

TEST_F(LineIOSegmentManipulatorTests, ScalarsParsing) {
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(&scalarA)
            .addScalarVariable(&scalarB)
            .addScalarVariable(&scalarC)
            .build();
    istream* in = new istringstream();

    {
        InSequence sequence;
        EXPECT_CALL(scalarA, parseFrom(in));
        EXPECT_CALL(whitespaceManipulator, parseSpace(in, "A"))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarB, parseFrom(in));
        EXPECT_CALL(whitespaceManipulator, parseSpace(in, "B"))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarC, parseFrom(in));
        EXPECT_CALL(whitespaceManipulator, parseNewline(in, "C"));
    }

    manipulator.parse(segment, in);
}

TEST_F(LineIOSegmentManipulatorTests, ScalarsPrinting) {
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(&scalarA)
            .addScalarVariable(&scalarB)
            .addScalarVariable(&scalarC)
            .build();
    ostream* out = new ostringstream();

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
