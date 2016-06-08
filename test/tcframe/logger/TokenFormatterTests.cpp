#include "gmock/gmock.h"

#include "tcframe/logger/TokenFormatter.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TokenFormatterTests : public Test {};

TEST_F(TokenFormatterTests, Variable) {
    EXPECT_THAT(TokenFormatter::formatVariable("X"), Eq("'X'"));
}

TEST_F(TokenFormatterTests, VectorElement) {
    EXPECT_THAT(TokenFormatter::formatVectorElement("X", 2), Eq("'X[2]'"));
}

TEST_F(TokenFormatterTests, MatrixElement) {
    EXPECT_THAT(TokenFormatter::formatMatrixElement("X", 2, 3), Eq("'X[2][3]'"));
}

}
