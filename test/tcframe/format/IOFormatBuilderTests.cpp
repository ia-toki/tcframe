#include "gmock/gmock.h"

#include "FakeIOSegment.hpp"
#include "tcframe/format/IOFormat.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class IOFormatBuilderTests : public Test {
protected:
    FakeIOSegment* segment1 = new FakeIOSegment();
    FakeIOSegment* segment2 = new FakeIOSegment();
    FakeIOSegment* segment3 = new FakeIOSegment();

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
