#include "gmock/gmock.h"

#include "tcframe/spec/io/GridIOSegment.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class GridIOSegmentBuilderTests : public Test {
protected:
    vector<vector<int>> M, N;

    GridIOSegmentBuilder builder;
};

TEST_F(GridIOSegmentBuilderTests, Building_Successful) {
    GridIOSegment* segment = builder
            .addMatrixVariable(Matrix::create(M, "M"))
            .setSize(2, 3)
            .build();

    EXPECT_TRUE(segment->variable()->equals(Matrix::create(M, "M")));
    EXPECT_THAT(segment->rows(), Eq(2));
    EXPECT_THAT(segment->columns(), Eq(3));
}

TEST_F(GridIOSegmentBuilderTests, Building_Failed_NoVariables) {
    try {
        builder
                .setSize(2, 3)
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Grid segment must have exactly one variable"));
    }
}

TEST_F(GridIOSegmentBuilderTests, Building_Failed_MultipleVariables) {
    try {
        builder
                .addMatrixVariable(Matrix::create(M, "M"))
                .addMatrixVariable(Matrix::create(N, "N"))
                .setSize(2, 3)
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Grid segment must have exactly one variable"));
    }
}

TEST_F(GridIOSegmentBuilderTests, Building_Failed_SizeNotSet) {
    try {
        builder
                .addMatrixVariable(Matrix::create(M, "M"))
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Grid segment must define matrix sizes"));
    }
}

}
