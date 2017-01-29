#include "gmock/gmock.h"

#include "tcframe/spec/io/RawLinesIOSegment.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class RawLinesIOSegmentBuilderTests : public Test {
protected:
    vector<string> V;

    RawLinesIOSegmentBuilder builder;
};

TEST_F(RawLinesIOSegmentBuilderTests, Building_Successful) {
    RawLinesIOSegment* segment = builder
            .addVectorVariable(Vector::createRaw(V, "V"))
            .setSize([] {return 2;})
            .build();

    EXPECT_TRUE(segment->variable()->equals(Vector::createRaw(V, "V")));
    EXPECT_THAT(segment->size()(), Eq(2));
}

TEST_F(RawLinesIOSegmentBuilderTests, Building_WithoutSize_Successful) {
    RawLinesIOSegment* segment = builder
            .addVectorVariable(Vector::createRaw(V, "V"))
            .build();

    EXPECT_TRUE(segment->variable()->equals(Vector::createRaw(V, "V")));
}

TEST_F(RawLinesIOSegmentBuilderTests, Building_Failed_NoVariables) {
    try {
        builder
                .setSize([] {return 2;})
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Raw lines segment must have exactly one variable"));
    }
}

TEST_F(RawLinesIOSegmentBuilderTests, Building_Failed_MultipleVariables) {
    try {
        builder
                .addVectorVariable(Vector::createRaw(V, "V"))
                .addVectorVariable(Vector::createRaw(V, "V"))
                .setSize([] {return 2;})
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Raw lines segment must have exactly one variable"));
    }
}

}
