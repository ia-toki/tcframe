#include "gmock/gmock.h"

#include "tcframe/validator/vector.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class VectorValidatorTests : public Test {};

TEST_F(VectorValidatorTests, eachElementIsBetween) {
    EXPECT_FALSE(eachElementIsBetween(vector<int>{2, 3, 1, 5, 4}, 1, 4));
    EXPECT_FALSE(eachElementIsBetween(vector<int>{2, 3, 1, 5, 4}, 2, 5));
    EXPECT_FALSE(eachElementIsBetween(vector<int>{2, 3, 1, 5, 4}, 2, 4));
    EXPECT_TRUE(eachElementIsBetween(vector<int>{2, 3, 1, 5, 4}, 1, 5));
    EXPECT_TRUE(eachElementIsBetween(vector<int>{2, 3, 1, 5, 4}, 0, 6));
}

}
