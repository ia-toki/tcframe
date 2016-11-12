#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int T;
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

    void Config() {
        MultipleTestCasesCount(T);
    }

    void MultipleTestCasesConstraints() {
        CONS(1 <= T && T <= 5);
    }

    void Constraints() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
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

    void TestGroup1() {
        CASE(A = 1, B = 3);
        CASE(A = 2, B = 4);
    }

    void TestGroup2() {
        CASE(A = 20, B = 30);
    }
};
