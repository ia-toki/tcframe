#include "gmock/gmock.h"

#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/io/LineIOSegmentScalarVariable.hpp"
#include "tcframe/type/Scalar.hpp"

using ::testing::ElementsAre;
using ::testing::Pointee;
using ::testing::Test;
using ::testing::WhenDynamicCastTo;

using tcframe::LineIOSegment;
using tcframe::LineIOSegmentScalarVariable;
using tcframe::LineIOSegmentBuilder;
using tcframe::Scalar;

class LineIOSegmentBuilderTests : public Test {
protected:
    int a;
    int b;
    LineIOSegmentBuilder builder;
};

TEST_F(LineIOSegmentBuilderTests, CanBuild) {
    LineIOSegment* segment = builder
            .addVariable(a, "a")
            .addVariable(b, "b")
            .build();

    EXPECT_THAT(segment->variables(), ElementsAre(
            WhenDynamicCastTo<LineIOSegmentScalarVariable*>(Pointee(LineIOSegmentScalarVariable(Scalar::create(a, "a")))),
            WhenDynamicCastTo<LineIOSegmentScalarVariable*>(Pointee(LineIOSegmentScalarVariable(Scalar::create(b, "b"))))));
}
