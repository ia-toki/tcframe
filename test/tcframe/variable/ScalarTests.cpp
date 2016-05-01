#include "gmock/gmock.h"

#include "tcframe/variable/Scalar.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class ScalarTests : public Test {
protected:
    int a;

    Scalar *A = Scalar::create(a, "a");
};

TEST_F(ScalarTests, Parsing) {
    istringstream sin("42");
    A->parseFrom(&sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ScalarTests, Printing) {
    a = 42;

    ostringstream sout;
    A->printTo(&sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}

}
