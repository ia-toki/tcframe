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

    void StyleConfig() {
        OutputOnlyEvaluator();
    }

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void TestCases() {
        CASE(A = 1, B = 2);
        CASE(A = 30, B = 40);
        CASE(A = 500, B = 600);
    }
};
