// A+B problem with MultipleTestCasesConfig — migrated from
// tcframe-python/examples/aplusb_multi.
//
// Input format:
//   T
//   A B       (T times)
//
// Output format:
//   Case #i: A+B     (T times)
//
// Build and generate test cases (from examples/aplusb_multi):
//   /path/to/tcframe-v2/scripts/tcframe make --solution=solutions/ref/solution.cpp

#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int T;
    int A, B;
    int res;

    void InputFormat() {
        LINE(A, B);
    }

    void OutputFormat() {
        LINE(res);
    }

    void GradingConfig() {
        TimeLimit(1);
        MemoryLimit(64);
    }

    void MultipleTestCasesConfig() {
        Counter(T);
        OutputPrefix("Case #%d: ");
    }

    void Constraints() {
        CONS(valueOf(A).isBetween(-1000000000, 1000000000));
        CONS(valueOf(B).isBetween(-1000000000, 1000000000));
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "1 2"
        });
        Output({
            "3"
        });
    }

    void SampleTestCase2() {
        Input({
            "-3 5"
        });
        Output({
            "2"
        });
    }

    void TestGroup1() {
        CASE(A = 1, B = 1);
        CASE(A = 0, B = 0);
        CASE(A = -1000000000, B = 1000000000);
        CASE(A = 1000000000, B = 1000000000);
        CASE(A = rnd.nextInt(-1000000000, 1000000000), B = rnd.nextInt(-1000000000, 1000000000));
    }
};
