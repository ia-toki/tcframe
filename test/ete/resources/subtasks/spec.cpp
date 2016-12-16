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
        CONS(A <= B);
    }

    void Subtask1() {
        CONS(1 <= A && A <= 10);
        CONS(1 <= B && B <= 10);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1, 2});
        Input({
            "1 5"
        });
        Output({
            "6"
        });
    }

    void SampleTestCase2() {
        Subtasks({2});
        Input({
            "11 12"
        });
        Output({
            "23"
        });
    }

    void TestGroup1() {
        Subtasks({1, 2});

        CASE(A = 1, B = 3);
        CASE(A = 2, B = 4);
    }

    void TestGroup2() {
        Subtasks({2});

        CASE(A = 20, B = 30);
    }
};
