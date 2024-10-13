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

TEST_F(VectorValidatorTests, elementsAreNonDescending) {
    EXPECT_FALSE(elementsAreNonDescending(vector<int>{1, 2, 3, 5, 3}));
    EXPECT_FALSE(elementsAreNonDescending(vector<int>{2, 1, 1, 2, 5}));
    EXPECT_TRUE(elementsAreNonDescending(vector<int>()));
    EXPECT_TRUE(elementsAreNonDescending(vector<int>{1, 2, 3, 4, 5}));
    EXPECT_TRUE(elementsAreNonDescending(vector<int>{1, 1, 2, 3, 3, 7}));
}

}
