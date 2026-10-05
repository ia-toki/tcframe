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
        CONS(1 <= A && A <= 100);
        CONS(A <= B);
    }

    void Subtask1() {
        Points(50);

        CONS(B <= 10);
    }

    void Subtask2() {
        Points(50);

        CONS(B <= 100);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void TestGroup1() {
        Subtasks({1, 2});

        CASE(A = 1, B = 5);
        CASE(A = 2, B = 10);
    }

    void TestGroup2() {
        Subtasks({2});

        CASE(A = 20, B = 100);
    }
};
