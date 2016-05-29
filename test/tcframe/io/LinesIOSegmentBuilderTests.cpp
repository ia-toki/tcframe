#include "gmock/gmock.h"

#include "tcframe/io/LinesIOSegment.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::SizeIs;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class LinesIOSegmentBuilderTests : public Test {
protected:
    vector<int> A;
    vector<int> B;
    vector<vector<int>> C;

    LinesIOSegmentBuilder builder;
};

TEST_F(LinesIOSegmentBuilderTests, Building) {
    LinesIOSegment* segment = builder
            .addVectorVariable(Vector::create(A, "A"))
            .addVectorVariable(Vector::create(B, "B"))
            .setSize(3)
            .build();

    ASSERT_THAT(segment->variables(), SizeIs(2));
    EXPECT_TRUE(segment->variables()[0]->equals(Vector::create(A, "A")));
    EXPECT_TRUE(segment->variables()[1]->equals(Vector::create(B, "B")));
    EXPECT_THAT(segment->size(), Eq(3));
}

TEST_F(LinesIOSegmentBuilderTests, Building_WithJaggedVector) {
    LinesIOSegment* segment = builder
            .addVectorVariable(Vector::create(A, "A"))
            .addVectorVariable(Vector::create(B, "B"))
            .addJaggedVectorVariable(Matrix::create(C, "C"))
            .setSize(4)
            .build();

    ASSERT_THAT(segment->variables(), SizeIs(3));
    EXPECT_TRUE(segment->variables()[0]->equals(Vector::create(A, "A")));
    EXPECT_TRUE(segment->variables()[1]->equals(Vector::create(B, "B")));
    EXPECT_TRUE(segment->variables()[2]->equals(Matrix::create(C, "C")));
    EXPECT_THAT(segment->size(), Eq(4));
}

TEST_F(LinesIOSegmentBuilderTests, Building_Failed_NoVariables) {
    try {
        builder
                .setSize(4)
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Lines segment must have at least one variable"));
    }
}

TEST_F(LinesIOSegmentBuilderTests, Building_Failed_JaggedVectorNotLast) {
    try {
        builder
                .addVectorVariable(Vector::create(B, "B"))
                .addJaggedVectorVariable(Matrix::create(C, "C"))
                .addVectorVariable(Vector::create(A, "A"))
                .setSize(5)
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Jagged vector can only be the last variable in a lines segment"));
    }
}

TEST_F(LinesIOSegmentBuilderTests, Building_Failed_SizeNotSet) {
    try {
        builder
                .addVectorVariable(Vector::create(A, "A"))
                .addVectorVariable(Vector::create(B, "B"))
                .addJaggedVectorVariable(Matrix::create(C, "C"))
                .build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Lines segment must define vector sizes"));
    }
}

}
