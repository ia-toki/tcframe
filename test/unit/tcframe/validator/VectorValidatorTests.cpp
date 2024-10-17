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

TEST_F(VectorValidatorTests, elementsAreNonAscending) {
    EXPECT_FALSE(elementsAreNonAscending(vector<int>{3, 5, 3, 2, 1}));
    EXPECT_FALSE(elementsAreNonAscending(vector<int>{5, 2, 1, 1, 2}));
    EXPECT_TRUE(elementsAreNonAscending(vector<int>()));
    EXPECT_TRUE(elementsAreNonAscending(vector<int>{5, 4, 3, 2, 1}));
    EXPECT_TRUE(elementsAreNonAscending(vector<int>{7, 3, 3, 2, 1, 1}));
}

TEST_F(VectorValidatorTests, elementsAreDescending) {
    EXPECT_FALSE(elementsAreDescending(vector<int>{3, 5, 3, 2, 1}));
    EXPECT_FALSE(elementsAreDescending(vector<int>{5, 2, 1, 1, 2}));
    EXPECT_FALSE(elementsAreDescending(vector<int>{7, 3, 3, 2, 1, 1}));
    EXPECT_TRUE(elementsAreDescending(vector<int>()));
    EXPECT_TRUE(elementsAreDescending(vector<int>{5, 4, 3, 2, 1}));
}

TEST_F(VectorValidatorTests, elementsAreAscending) {
    EXPECT_FALSE(elementsAreAscending(vector<int>{1, 2, 3, 5, 3}));
    EXPECT_FALSE(elementsAreAscending(vector<int>{2, 1, 1, 2, 5}));
    EXPECT_FALSE(elementsAreAscending(vector<int>{1, 1, 2, 3, 3, 7}));
    EXPECT_TRUE(elementsAreAscending(vector<int>()));
    EXPECT_TRUE(elementsAreAscending(vector<int>{1, 2, 3, 4, 5}));
}

}
