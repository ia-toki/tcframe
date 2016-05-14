#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/io/LineIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class LineIOSegmentManipulatorTests : public Test {};

TEST_F(LineIOSegmentManipulatorTests, Parsing_SingleScalar) {
    int X;
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(X, "X"))
            .build();
    istringstream in("42\n");

    LineIOSegmentManipulator::parse(segment, &in);
    EXPECT_THAT(X, Eq(42));
}

TEST_F(LineIOSegmentManipulatorTests, Printing_SingleScalar) {
    int X = 42;
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(Scalar::create(X, "X"))
            .build();
    ostringstream out;

    LineIOSegmentManipulator::print(segment, &out);
    EXPECT_THAT(out.str(), Eq("42\n"));
}

}
