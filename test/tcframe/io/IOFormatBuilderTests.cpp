#include "gmock/gmock.h"

#include "tcframe/io/IOFormat.hpp"
#include "tcframe/io/IOSegment.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

struct FakeIOSegment : public IOSegment {
public:
    IOSegmentType type() {
        return IOSegmentType::LINE;
    }
};

class IOFormatBuilderTests : public Test {
protected:
    FakeIOSegment* segment1;
    FakeIOSegment* segment2;
    FakeIOSegment* segment3;

    IOFormatBuilder builder;

    IOFormatBuilderTests()
            : segment1(new FakeIOSegment())
            , segment2(new FakeIOSegment())
            , segment3(new FakeIOSegment())
    {}
};

TEST_F(IOFormatBuilderTests, CanBuild) {
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
