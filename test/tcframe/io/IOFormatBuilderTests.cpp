#include "gmock/gmock.h"

#include "tcframe/io/IOFormat.hpp"
#include "tcframe/io/LineIOSegment.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class IOFormatBuilderTests : public Test {
protected:
    LineIOSegment* segment1 = LineIOSegmentBuilder().build();
    LineIOSegment* segment2 = LineIOSegmentBuilder().build();
    LineIOSegment* segment3 = LineIOSegmentBuilder().build();

    IOFormatBuilder builder;
};

TEST_F(IOFormatBuilderTests, Building) {
    IOFormat ioFormat = builder
            .prepareForInputFormat()
            .addIOSegment(segment1)
            .addIOSegment(segment2)
            .prepareForOutputFormat()
            .addIOSegment(segment3)
            .build();

    EXPECT_THAT(ioFormat.inputFormat(), ElementsAre(
            segment1, segment2));

    EXPECT_THAT(ioFormat.outputFormat(), ElementsAre(
            segment3));
}

}
