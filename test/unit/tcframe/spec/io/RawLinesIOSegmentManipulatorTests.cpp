#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/spec/io/RawLinesIOSegmentManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class RawLinesIOSegmentManipulatorTests : public Test {
protected:
    vector<string> V;
    function<int()> size = [] {return 2;};

    RawLinesIOSegment* segment = RawLinesIOSegmentBuilder()
            .addVectorVariable(Vector::createRaw(V, "V"))
            .setSize(size)
            .build();
    RawLinesIOSegment* segmentWithoutSize = RawLinesIOSegmentBuilder()
            .addVectorVariable(Vector::createRaw(V, "V"))
            .build();
    
    RawLinesIOSegmentManipulator manipulator;
};

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_Successful) {
    istringstream in("hello, world!\n  lorem  ipsum \n");

    manipulator.parse(segment, &in);
    EXPECT_THAT(V, Eq(vector<string>{"hello, world!", "  lorem  ipsum "}));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_Successful_CheckLastVariable) {
    istringstream in("hello, world!\n  lorem  ipsum \n");

    EXPECT_THAT(manipulator.parse(segment, &in), Eq("'V[1]'"));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_Failed_MissingNewline) {
    istringstream in("hello, world!\n  lorem  ipsum ");

    try {
        manipulator.parse(segment, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'V[1]'"));
    }
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_WithoutSize_Successful) {
    istringstream in("hello, world!\n  lorem  ipsum \n");

    manipulator.parse(segmentWithoutSize, &in);
    EXPECT_THAT(V, Eq(vector<string>{"hello, world!", "  lorem  ipsum "}));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_WithoutSize_Successful_CheckLastVariable) {
    istringstream in("hello, world!\n  lorem  ipsum \n");

    EXPECT_THAT(manipulator.parse(segmentWithoutSize, &in), Eq("'V[1]'"));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_WithoutSize_Failed_MissingNewline) {
    istringstream in("hello, world!\n  lorem  ipsum ");

    try {
        manipulator.parse(segmentWithoutSize, &in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <newline> after 'V[1]'"));
    }
}

TEST_F(RawLinesIOSegmentManipulatorTests, Printing_Successful) {
    ostringstream out;

    V = {"hello, world!", "  lorem  ipsum "};

    manipulator.print(segment, &out);
    EXPECT_THAT(out.str(), Eq("hello, world!\n  lorem  ipsum \n"));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Printing_Failed_SizeMismatch) {
    ostringstream out;

    V = {"hello, world!"};

    try {
        manipulator.print(segment, &out);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Number of elements of 'V' unsatisfied. Expected: 2, actual: 1"));
    }
}

TEST_F(RawLinesIOSegmentManipulatorTests, Printing_WithoutSize_Successful) {
    ostringstream out;

    V = {"hello, world!", "  lorem  ipsum "};

    manipulator.print(segmentWithoutSize, &out);
    EXPECT_THAT(out.str(), Eq("hello, world!\n  lorem  ipsum \n"));
}

TEST_F(RawLinesIOSegmentManipulatorTests, Parsing_ClearSegment_Successful) {
    istringstream in("hello, world!\n  lorem  ipsum \n");

    manipulator.parse(segment, &in);

    in.seekg(0, in.beg);
    manipulator.parse(segment, &in);
    EXPECT_THAT(V, Eq(vector<string>{"hello, world!", "  lorem  ipsum "}));
}
}
