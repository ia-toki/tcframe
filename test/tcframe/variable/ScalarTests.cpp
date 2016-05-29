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
    Scalar* A = Scalar::create(a, "a");
};

TEST_F(ScalarTests, Parsing) {
    istringstream in("42");
    A->parseFrom(&in);
    EXPECT_THAT(a, Eq(42));
}

TEST_F(ScalarTests, Printing) {
    ostringstream out;
    a = 42;
    A->printTo(&out);
    EXPECT_THAT(out.str(), Eq("42"));
}

}
