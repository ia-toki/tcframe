#include "gmock/gmock.h"

#include "tcframe/type/Scalar.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::Test;

using tcframe::Scalar;

class ScalarTests : public Test {
protected:
    int a;
    Scalar* A;

    ScalarTests()
            : A(Scalar::create(a, "a"))
    {}
};

TEST_F(ScalarTests, CanAccessName) {
    EXPECT_THAT(A->getName(), Eq("a"));
}

TEST_F(ScalarTests, CanParse) {
    istringstream sin("42");
    A->parseFrom(sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ScalarTests, CanPrint) {
    a = 42;

    ostringstream sout;
    A->printTo(sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}
