#include "gmock/gmock.h"

#include "tcframe/spec/io/RawLineIOSegment.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class RawLineIOSegmentBuilderTests : public Test {
protected:
    string S;

    RawLineIOSegmentBuilder builder;
};

TEST_F(RawLineIOSegmentBuilderTests, Building_Successful) {
    RawLineIOSegment* segment = builder
            .addScalarVariable(Scalar::createRaw(S, "S"))
            .build();

    EXPECT_TRUE(segment->variable()->equals(Scalar::createRaw(S, "S")));
}

TEST_F(RawLineIOSegmentBuilderTests, Building_Failed_NoVariables) {
    try {
        builder.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Raw line segment must have exactly one variable"));
    }
}

TEST_F(RawLineIOSegmentBuilderTests, Building_Failed_MultipleVariables) {
    try {
        builder
                .addScalarVariable(Scalar::createRaw(S, "S"))
                .addScalarVariable(Scalar::createRaw(S, "S"))
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Raw line segment must have exactly one variable"));
    }
}

}
