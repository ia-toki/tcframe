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

    void StyleConfig() {
        NoOutput();
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void MultipleTestCasesConfig() {
        Counter(T);
    }

    void MultipleTestCasesConstraints() {
        CONS(valueOf(T).isBetween(1, 5));
    }

    void Constraints() {
        CONS(valueOf(A).isBetween(1, 100));
        CONS(valueOf(B).isBetween(1, 100));
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "1 5"
        });
    }

    void SampleTestCase2() {
        Input({
            "4 7"
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
