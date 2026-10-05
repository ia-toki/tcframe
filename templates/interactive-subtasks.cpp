#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;

    void InputFormat() {
        LINE(N);
    }

    void StyleConfig() {
        InteractiveEvaluator();
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void Constraints() {
        CONS(valueOf(N).isBetween(1, 1000));
    }

    void Subtask1() {
        Points(50);

        CONS(valueOf(N).isBetween(1, 10));
    }

    void Subtask2() {
        Points(50);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1, 2});
        Input({
            "3"
        });
    }

    void TestGroup1() {
        Subtasks({1, 2});

        CASE(N = 1);
        CASE(N = 2);
        CASE(N = 10);
    }

    void TestGroup2() {
        Subtasks({2});

        CASE(N = 100);
        CASE(N = 1000);
    }
};
