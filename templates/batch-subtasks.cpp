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
        CONS(1 <= A && A <= 1000000000);
        CONS(1 <= B && B <= 1000000000);
    }

    void Subtask1() {
        Points(40);

        CONS(1 <= A && A <= 10);
        CONS(1 <= B && B <= 10);
    }

    void Subtask2() {
        Points(60);
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

    void TestGroup1() {
        Subtasks({1, 2});

        CASE(A = 1, B = 1);
        CASE(A = 2, B = 4);
        CASE(A = rnd.nextInt(1, 10), B = rnd.nextInt(1, 10));
    }

    void TestGroup2() {
        Subtasks({2});

        CASE(A = 1000000000, B = 1000000000);
        CASE(A = rnd.nextInt(1, 1000000000), B = rnd.nextInt(1, 1000000000));
    }
};
