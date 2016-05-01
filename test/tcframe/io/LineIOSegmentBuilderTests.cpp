#include "gmock/gmock.h"

#include "tcframe/io/LineIOSegment.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class LineIOSegmentBuilderTests : public Test {
protected:
    int a;
    int b;

    LineIOSegmentBuilder builder;
};

TEST_F(LineIOSegmentBuilderTests, Building) {
    LineIOSegment* segment = builder
            .addVariable(a, "a")
            .addVariable(b, "b")
            .build();

    EXPECT_THAT(segment->variables(), ElementsAre(
            LineIOSegmentVariable(Scalar::create(a, "a")),
            LineIOSegmentVariable(Scalar::create(a, "b"))));
}

}
