#include "gmock/gmock.h"

#include "tcframe/spec/io/IOFormat.hpp"

using ::testing::A;
using ::testing::ElementsAre;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class IOFormatBuilderTests : public Test {
protected:
    int X;
    vector<int> Y;
    vector<vector<int>> Z;

    IOFormatBuilder builder;
};

TEST_F(IOFormatBuilderTests, Building_Successful) {
    builder.prepareForInputFormat();
    builder
            .newLineIOSegment()
            .addScalarVariable(Scalar::create(X, "X"));
    builder
            .newLinesIOSegment()
            .addVectorVariable(Vector::create(Y, "Y"))
            .setSize(new int(3));
    builder
            .newGridIOSegment()
            .addMatrixVariable(Matrix::create(Z, "Z"))
            .setSize(new int(2), new int(3))
            .build();

    builder.prepareForOutputFormat();
    builder
            .newGridIOSegment()
            .addMatrixVariable(Matrix::create(Z, "Z"))
            .setSize(new int(2), new int(3))
            .build();
    builder
            .newLinesIOSegment()
            .addVectorVariable(Vector::create(Y, "Y"))
            .setSize(new int(3));
    builder
            .newLineIOSegment()
            .addScalarVariable(Scalar::create(X, "X"));
    IOFormat ioFormat = builder.build();

    EXPECT_THAT(ioFormat.inputFormat(), ElementsAre(
            A<LineIOSegment*>(),
            A<LinesIOSegment*>(),
            A<GridIOSegment*>()));

    EXPECT_THAT(ioFormat.outputFormat(), ElementsAre(
            A<GridIOSegment*>(),
            A<LinesIOSegment*>(),
            A<LineIOSegment*>()));
}

TEST_F(IOFormatBuilderTests, Building_Failed_LinesSegmentWithoutSizeNotLast) {
    try {
        builder.prepareForInputFormat();
        builder
                .newLineIOSegment()
                .addScalarVariable(Scalar::create(X, "X"));
        builder
                .newLinesIOSegment()
                .addVectorVariable(Vector::create(Y, "Y"));
        builder
                .newGridIOSegment()
                .addMatrixVariable(Matrix::create(Z, "Z"))
                .setSize(new int(2), new int(3));
        builder.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Lines segment without size can only be the last segment"));
    }

    try {
        builder.prepareForOutputFormat();
        builder
                .newLineIOSegment()
                .addScalarVariable(Scalar::create(X, "X"));
        builder
                .newLinesIOSegment()
                .addVectorVariable(Vector::create(Y, "Y"));
        builder
                .newGridIOSegment()
                .addMatrixVariable(Matrix::create(Z, "Z"))
                .setSize(new int(2), new int(3));
        builder.build();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Lines segment without size can only be the last segment"));
    }
}

}
