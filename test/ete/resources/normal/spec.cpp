#include <tcframe/spec.hpp>
#include <library/library.hpp>

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
        CONS(valueOf(A).isBetween(1, 10));
        CONS(valueOf(B).isBetween(1, 10));
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
