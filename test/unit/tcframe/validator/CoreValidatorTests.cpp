#include "gmock/gmock.h"

#include "tcframe/validator/core.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class CoreValidatorTests : public Test {};

TEST_F(CoreValidatorTests, valueOf_isBetween) {
    EXPECT_FALSE(valueOf(5).isBetween(1, 4));
    EXPECT_FALSE(valueOf(5).isBetween(6, 10));
    EXPECT_FALSE(valueOf(5).isBetween(100, -100));
    EXPECT_TRUE(valueOf(5).isBetween(1, 5));
    EXPECT_TRUE(valueOf(5).isBetween(5, 10));
    EXPECT_TRUE(valueOf(5).isBetween(0, 10));
}

TEST_F(CoreValidatorTests, eachElementOf_isBetween) {
    EXPECT_FALSE(eachElementOf(vector<int>{2, 3, 1, 5, 4}).isBetween(1, 4));
    EXPECT_FALSE(eachElementOf(vector<int>{2, 3, 1, 5, 4}).isBetween(2, 5));
    EXPECT_FALSE(eachElementOf(vector<int>{2, 3, 1, 5, 4}).isBetween(2, 4));
    EXPECT_TRUE(eachElementOf(vector<int>{2, 3, 1, 5, 4}).isBetween(1, 5));
    EXPECT_TRUE(eachElementOf(vector<int>{2, 3, 1, 5, 4}).isBetween(0, 6));

    EXPECT_TRUE(eachElementOf(vector<int>{}).isBetween(0, 6));

    EXPECT_FALSE(eachElementOf(vector<vector<int>>{
        vector<int>{1, 2},
        vector<int>{3, 4}}).isBetween(2, 4));
    EXPECT_FALSE(eachElementOf(vector<vector<int>>{
        vector<int>{1, 2},
        vector<int>{3, 4}}).isBetween(1, 3));
    EXPECT_TRUE(eachElementOf(vector<vector<int>>{
        vector<int>{1, 2},
        vector<int>{3, 4}}).isBetween(1, 4));
}

TEST_F(CoreValidatorTests, elementsOf_areAscending) {
    EXPECT_FALSE(elementsOf(vector<int>{1, 2, 3, 5, 3}).areAscending());
    EXPECT_FALSE(elementsOf(vector<int>{2, 1, 1, 2, 5}).areAscending());
    EXPECT_FALSE(elementsOf(vector<int>{1, 1, 2, 3, 3, 7}).areAscending());
    EXPECT_TRUE(elementsOf(vector<int>()).areAscending());
    EXPECT_TRUE(elementsOf(vector<int>{1, 2, 3, 4, 5}).areAscending());
}

TEST_F(CoreValidatorTests, elementsOf_areDescending) {
    EXPECT_FALSE(elementsOf(vector<int>{3, 5, 3, 2, 1}).areDescending());
    EXPECT_FALSE(elementsOf(vector<int>{5, 2, 1, 1, 2}).areDescending());
    EXPECT_FALSE(elementsOf(vector<int>{7, 3, 3, 2, 1, 1}).areDescending());
    EXPECT_TRUE(elementsOf(vector<int>()).areDescending());
    EXPECT_TRUE(elementsOf(vector<int>{5, 4, 3, 2, 1}).areDescending());
}

TEST_F(CoreValidatorTests, elementsOf_areNonAscending) {
    EXPECT_FALSE(elementsOf(vector<int>{3, 5, 3, 2, 1}).areNonAscending());
    EXPECT_FALSE(elementsOf(vector<int>{5, 2, 1, 1, 2}).areNonAscending());
    EXPECT_TRUE(elementsOf(vector<int>()).areNonAscending());
    EXPECT_TRUE(elementsOf(vector<int>{5, 4, 3, 2, 1}).areNonAscending());
    EXPECT_TRUE(elementsOf(vector<int>{7, 3, 3, 2, 1, 1}).areNonAscending());
}

TEST_F(CoreValidatorTests, elementsOf_areNonDescending) {
    EXPECT_FALSE(elementsOf(vector<int>{1, 2, 3, 5, 3}).areNonDescending());
    EXPECT_FALSE(elementsOf(vector<int>{2, 1, 1, 2, 5}).areNonDescending());
    EXPECT_TRUE(elementsOf(vector<int>()).areNonDescending());
    EXPECT_TRUE(elementsOf(vector<int>{1, 2, 3, 4, 5}).areNonDescending());
    EXPECT_TRUE(elementsOf(vector<int>{1, 1, 2, 3, 3, 7}).areNonDescending());
}

TEST_F(CoreValidatorTests, elementsOf_areUnique) {
    EXPECT_FALSE(elementsOf(vector<int>{5, 1, 3, 4, 2, 1}).areUnique());
    EXPECT_FALSE(elementsOf(vector<char>{'a', 'c', 'f', 'f', 'd'}).areUnique());
    EXPECT_TRUE(elementsOf(vector<int>()).areUnique());
    EXPECT_TRUE(elementsOf(vector<int>{5, 2, 4, 1, 9}).areUnique());
    EXPECT_TRUE(elementsOf(vector<char>{'a', 'x', 'd', 'g', 'h'}).areUnique());
}

TEST_F(CoreValidatorTests, eachCharacterOf_isBetween) {
    EXPECT_FALSE(eachCharacterOf("BCDEF").isBetween('B', 'E'));
    EXPECT_FALSE(eachCharacterOf("BCDEF").isBetween('C', 'F'));
    EXPECT_TRUE(eachCharacterOf("BCDEF").isBetween('B', 'F'));
    EXPECT_TRUE(eachCharacterOf("BCDEF").isBetween('A', 'G'));
}

}
