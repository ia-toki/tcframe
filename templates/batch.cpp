#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int A, B;
    int res;

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
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "1 5"
        });
        Output({
            "6"
        });
    }

    void TestCases() {
        CASE(A = 1, B = 1);
        CASE(A = 77, B = 99);
        CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
    }
};
