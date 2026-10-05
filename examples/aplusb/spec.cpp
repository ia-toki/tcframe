// A+B Problem — migrated from tcframe-python/examples/aplusb.
//
// Build and generate test cases (from examples/aplusb):
//   /path/to/tcframe-v2/scripts/tcframe make --solution=solutions/ref/solution.cpp
//
// Package:
//   /path/to/tcframe-v2/scripts/tcframe package --solution=solutions/ref/solution.cpp

#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    long long A, B;
    long long res;

    void InputFormat() {
        LINE(A, B);
    }

    void OutputFormat() {
        LINE(res);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void Constraints() {
        CONS(1 <= A && A <= 1000000000000000000LL);
        CONS(1 <= B && B <= 1000000000000000000LL);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "2 8"
        });
        Output({
            "10"
        });
    }

    void SampleTestCase2() {
        Input({
            "1000000000000000000 1"
        });
        Output({
            "1000000000000000001"
        });
    }

    void TestGroup1() {
        // Edge cases
        CASE(A = 1, B = 1);
        CASE(A = 1, B = 1000000000000000000LL);
        CASE(A = 1000000000000000000LL, B = 1000000000000000000LL);

        // Random small
        for (int i = 0; i < 3; i++) {
            CASE(A = rnd.nextLongLong(1, 1000), B = rnd.nextLongLong(1, 1000));
        }

        // Random large
        for (int i = 0; i < 3; i++) {
            CASE(A = rnd.nextLongLong(1, 1000000000000000000LL), B = rnd.nextLongLong(1, 1000000000000000000LL));
        }

        CASE(A = 1000000000000000000LL, B = 999999999999999999LL);
    }
};
