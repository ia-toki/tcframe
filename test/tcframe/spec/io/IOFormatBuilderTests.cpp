#include "gmock/gmock.h"

#include "tcframe/spec/io/IOFormat.hpp"

using ::testing::A;
using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class IOFormatBuilderTests : public Test {
protected:
    IOFormatBuilder builder;
};

TEST_F(IOFormatBuilderTests, Building) {
    builder.prepareForInputFormat();
    builder.newLineIOSegment();
    builder.newLineIOSegment();
    builder.prepareForOutputFormat();
    builder.newLineIOSegment();
    IOFormat ioFormat = builder.build();

    EXPECT_THAT(ioFormat.inputFormat(), ElementsAre(
            A<LineIOSegment*>(),
            A<LineIOSegment*>()));

    EXPECT_THAT(ioFormat.outputFormat(), ElementsAre(
            A<LineIOSegment*>()));
}

}
