#include "gtest/gtest.h"

#include "tcframe/random.hpp"
#include <vector>

using std::vector;
using tcframe::Random;

TEST(RandomTest, NextNumberSingleArgument) {
    Random rnd;

    int a = rnd.nextInt(10);
    EXPECT_GE(a, 0);
    EXPECT_LT(a, 10);

    long long b = rnd.nextLongLong(1000000000000ll);
    EXPECT_GE(b, 0);
    EXPECT_LT(b, 1000000000000ll);

    double c = rnd.nextDouble(100.0);
    EXPECT_GE(c, 0);
    EXPECT_LE(c, 100.0);
}

TEST(RandomTest, NextNumberTwoArgument) {
    Random rnd;

    int a = rnd.nextInt(10, 20);
    EXPECT_GE(a, 10);
    EXPECT_LT(a, 20);

    long long b = rnd.nextLongLong(1000000000000ll, 2000000000000ll);
    EXPECT_GE(b, 1000000000000ll);
    EXPECT_LT(b, 2000000000000ll);

    double c = rnd.nextDouble(100.0, 200.0);
    EXPECT_GE(c, 100.0);
    EXPECT_LE(c, 200.0);
}

TEST(RandomTest, Shuffle) {
    Random rnd;

    vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rnd.shuffle(data.begin(), data.end());

    // almost always true
    EXPECT_NE((vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), data);

    sort(data.begin(), data.end());
    EXPECT_EQ((vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), data);
}

TEST(RandomTest, Seeding) {
    Random rnd1;
    int a1 = rnd1.nextInt(12345);

    Random rnd2;
    rnd2.setSeed(42);
    int a2 = rnd2.nextInt(12345);

    // almost always true
    EXPECT_NE(a1, a2);
}
