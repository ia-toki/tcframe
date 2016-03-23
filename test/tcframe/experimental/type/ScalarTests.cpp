#include "gmock/gmock.h"

#include "tcframe/experimental/type/Scalar.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::Test;

using tcframe::experimental::Scalar;

class ExperimentalScalarTest : public Test {

};

TEST_F(ExperimentalScalarTest, CanAccessName) {
    int a;
    Scalar A(a, "a");

    EXPECT_THAT(A.name(), Eq("a"));
}

TEST_F(ExperimentalScalarTest, CanUseParseFunction) {
    int a;
    Scalar A(a, "a");

    istringstream sin("42");
    A.parseFunction()(sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ExperimentalScalarTest, CanUsePrintFunction) {
    int a = 42;
    Scalar A(a, "a");

    ostringstream sout;
    A.printFunction()(sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}
