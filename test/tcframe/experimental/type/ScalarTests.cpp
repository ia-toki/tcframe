#include "gmock/gmock.h"

#include "tcframe/experimental/type/Scalar.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::Test;

using tcframe::experimental::Scalar;

class ExperimentalScalarTest : public Test {
protected:
    int a;
    Scalar* A;

    ExperimentalScalarTest()
            : A(Scalar::create(a, "a"))
    {}
};

TEST_F(ExperimentalScalarTest, CanAccessName) {
    EXPECT_THAT(A->getName(), Eq("a"));
}

TEST_F(ExperimentalScalarTest, CanParse) {
    istringstream sin("42");
    A->parseFrom(sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ExperimentalScalarTest, CanPrint) {
    a = 42;

    ostringstream sout;
    A->printTo(sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}
