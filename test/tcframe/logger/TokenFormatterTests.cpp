#include "gmock/gmock.h"

#include "tcframe/logger/TokenFormatter.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class TokenFormatterTests : public Test {};

TEST_F(TokenFormatterTests, Name) {
    EXPECT_THAT(TokenFormatter::formatVariable("X"), Eq("'X'"));
}

TEST_F(TokenFormatterTests, VectorElementName) {
    EXPECT_THAT(TokenFormatter::formatVectorElement("X", 2), Eq("'X[2]'"));
}

TEST_F(TokenFormatterTests, MatrixElementName) {
    EXPECT_THAT(TokenFormatter::formatMatrixElement("X", 2, 3), Eq("'X[2][3]'"));
}

}
