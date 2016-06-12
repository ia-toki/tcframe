#include "gmock/gmock.h"

#include "tcframe/random/Random.hpp"

#include <algorithm>
#include <vector>

using std::vector;

using ::testing::Eq;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Ge;
using ::testing::Ne;
using ::testing::Test;

namespace tcframe {

class RandomTests : public Test {
protected:
    Random rnd;
};

TEST_F(RandomTests, NextNumber_SingleArgument) {
    int a = rnd.nextInt(10);
    EXPECT_THAT(a, Ge(0));
    EXPECT_THAT(a, Lt(10));

    long long b = rnd.nextLongLong(1000000000000ll);
    EXPECT_THAT(b, Ge(0));
    EXPECT_THAT(b, Lt(1000000000000ll));

    double c = rnd.nextDouble(100.0);
    EXPECT_THAT(c, Ge(0.0));
    EXPECT_THAT(c, Le(100.0));
}

TEST_F(RandomTests, NextNumber_TwoArgument) {
    int a = rnd.nextInt(10, 20);
    EXPECT_THAT(a, Ge(10));
    EXPECT_THAT(a, Lt(20));

    long long b = rnd.nextLongLong(1000000000000ll, 2000000000000ll);
    EXPECT_THAT(b, Ge(1000000000000ll));
    EXPECT_THAT(b, Lt(2000000000000ll));

    double c = rnd.nextDouble(100.0, 200.0);
    EXPECT_THAT(c, Ge(100.0));
    EXPECT_THAT(c, Le(200.0));
}

TEST_F(RandomTests, Shuffle) {
    vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rnd.shuffle(data.begin(), data.end());

    // almost always true
    EXPECT_THAT(data, Ne(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));

    sort(data.begin(), data.end());
    EXPECT_THAT(data, Eq(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

TEST_F(RandomTests, Seed) {
    int a = rnd.nextInt(12345);

    Random rnd2;
    rnd2.setSeed(42);
    int b = rnd2.nextInt(12345);

    // almost always true
    EXPECT_THAT(a, Ne(b));
}

}
