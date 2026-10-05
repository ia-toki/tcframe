#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int A, B;
    double res;

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
        CONS(1 <= B && B <= 100);
    }

    void StyleConfig() {
        BatchEvaluator();
        AbsoluteFloatToleranceScorer(6);
    }

    void Subtask1() {
        Points(40);
        ThresholdAggregator(18);

        CONS(A <= 10);
        CONS(B <= 10);
    }

    void Subtask2() {
        Points(60);
        MinAggregator();
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void TestGroup1() {
        Subtasks({1, 2});

        CASE(A = 1, B = 3);
        CASE(A = 2, B = 3);
    }

    void TestGroup2() {
        Subtasks({2});

        CASE(A = 37, B = 91);
        CASE(A = 100, B = 7);
    }
};
