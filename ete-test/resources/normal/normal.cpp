#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int A, B;
    int res;

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void InputFormat() {
        LINE(A, B);
    }

    void OutputFormat() {
        LINE(res);
    }

    void Constraints() {
        CONS(1 <= A && A <= 10);
        CONS(1 <= B && B <= 10);
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
        CASE(A = 1, B = 3);
        CASE(A = 2, B = 4);
        CASE(A = 5, B = 6);
    }
};
