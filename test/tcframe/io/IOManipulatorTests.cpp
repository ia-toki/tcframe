#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/io/IOManipulator.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class IOManipulatorTests : public Test {
protected:
    int A;
    int B;

    IOManipulator* manipulator;

    void SetUp() {
        IOFormatBuilder ioFormatBuilder;
        ioFormatBuilder.prepareForInputFormat();
        ioFormatBuilder.newLineIOSegment()
                .addScalarVariable(Scalar::create(A, "A"));
        ioFormatBuilder.newLineIOSegment()
                .addScalarVariable(Scalar::create(B, "B"));
        IOFormat ioFormat = ioFormatBuilder.build();

        manipulator = new IOManipulator(ioFormat);
    }
};

TEST_F(IOManipulatorTests, Parsing_Successful) {
    istringstream in("123\n42\n");
    manipulator->parseInput(&in);
    EXPECT_THAT(A, Eq(123));
    EXPECT_THAT(B, Eq(42));
}

TEST_F(IOManipulatorTests, Parsing_Failed_MissingEof) {
    istringstream in("123\n42\nbogus");
    try {
        manipulator->parseInput(&in);
        FAIL();
    } catch(runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <EOF>"));
    }
}

TEST_F(IOManipulatorTests, Printing_Successful) {
    A = 123;
    B = 42;
    ostringstream out;
    manipulator->printInput(&out);
    EXPECT_THAT(out.str(), Eq("123\n42\n"));
}

}
