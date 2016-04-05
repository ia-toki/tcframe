#include "gmock/gmock.h"

#include "tcframe/experimental/type/Scalar.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::Test;

using tcframe::experimental::ScalarImpl;

class ExperimentalScalarImplTest : public Test {
protected:
    int a;
    ScalarImpl<int> A;

    ExperimentalScalarImplTest()
            : A(ScalarImpl<int>(a, "a"))
    {}
};

TEST_F(ExperimentalScalarImplTest, CanAccessName) {
    EXPECT_THAT(A.getName(), Eq("a"));
}

TEST_F(ExperimentalScalarImplTest, CanParse) {
    istringstream sin("42");
    A.parseFrom(sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ExperimentalScalarImplTest, CanPrint) {
    a = 42;

    ostringstream sout;
    A.printTo(sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}
