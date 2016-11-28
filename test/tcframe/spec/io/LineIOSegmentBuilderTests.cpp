#include "gmock/gmock.h"

#include "tcframe/spec/io/LineIOSegment.hpp"

using ::testing::ElementsAre;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class LineIOSegmentBuilderTests : public Test {
protected:
    int A;
    vector<int> B;
    vector<int> C;

    LineIOSegmentBuilder builder;
};

TEST_F(LineIOSegmentBuilderTests, Building_Successful) {
    LineIOSegment* segment = builder
            .addScalarVariable(Scalar::create(A, "A"))
            .addVectorVariable(Vector::create(B, "B"), new int(7))
            .addVectorVariable(Vector::create(C, "C"))
            .build();

    EXPECT_THAT(segment->variables(), ElementsAre(
            LineIOSegmentVariable(Scalar::create(A, "A")),
            LineIOSegmentVariable(Vector::create(B, "B"), new int(7)),
            LineIOSegmentVariable(Vector::create(C, "C"))));
}

TEST_F(LineIOSegmentBuilderTests, Building_Failed_VectorWithoutSizeNotLast) {
    try {
        builder
                .addScalarVariable(Scalar::create(A, "A"))
                .addVectorVariable(Vector::create(B, "B"))
                .addVectorVariable(Vector::create(C, "C"), new int(7))
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Vector without size can only be the last variable in a line segment"));
    }
}

}
