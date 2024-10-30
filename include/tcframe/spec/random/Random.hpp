#pragma once

#include <algorithm>
#include <random>

using std::mt19937;
using std::mt19937_64;
using std::random_shuffle;
using std::uniform_real_distribution;
using std::uniform_int_distribution;

namespace tcframe {

class Random {
public:
    void setSeed(unsigned int seed) {
        engine.seed(seed);
    }

    int nextInt(int minNum, int maxNum) {
        uniform_int_distribution<int> distr(minNum, maxNum);
        return distr(engine);
    }

    int nextInt(int maxNumEx) {
        return nextInt(0, maxNumEx - 1);
    }

    long long nextLongLong(long long minNum, long long maxNum) {
        uniform_int_distribution<long long> distr(minNum, maxNum);
        return distr(engine);
    }

    long long nextLongLong(long long maxNumEx) {
        return nextLongLong(0, maxNumEx - 1);
    }

    double nextDouble(double minNum, double maxNum) {
        uniform_real_distribution<double> distr(minNum, maxNum);
        return distr(engine);
    }

    double nextDouble(double maxNum) {
        return nextDouble(0, maxNum);
    }

    template<typename RandomAccessIterator>
    void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        std::shuffle(first, last, engine);
    }

    mt19937_64 *getEngine() {
        return &engine;
    }

private:
    mt19937_64 engine;
};

}
