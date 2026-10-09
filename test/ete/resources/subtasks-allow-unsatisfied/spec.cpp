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

    void Constraints() {
        CONS(A <= B);
    }

    void Subtask1() {
        Points(70);

        CONS(1 <= A && A <= 10);
        CONS(1 <= B && B <= 10);
    }

    void Subtask2() {
        Points(30);

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

    void TestGroup1() {
        Subtasks({1});

        CASE(A = 1, B = 3);
    }

    // Also satisfies subtask 1, although it is only assigned to subtask 2
    void TestGroup2() {
        Subtasks({2});

        CASE(A = 2, B = 4);
    }
};
