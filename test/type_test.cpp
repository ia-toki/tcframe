#include "gtest/gtest.h"

#include "tcframe/type.h"

#include <sstream>

using std::ostringstream;
using tcframe::Scalar;

TEST(ScalarTest, Printing) {
    int x;
    Scalar<int> wrappedX(x);

    x = 42;
    ostringstream sout;
    wrappedX.printTo(sout);

    EXPECT_EQ("42", sout.str());
}
