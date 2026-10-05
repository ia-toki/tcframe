#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    double X;

    void InputFormat() {
        LINE(N);
    }

    void OutputFormat() {
        LINE(X);
    }

    void StyleConfig() {
        BatchEvaluator();
        AbsoluteFloatToleranceScorer(6);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void Constraints() {
        CONS(valueOf(N).isBetween(1, 10));
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "3"
        });
        Output({
            "1.5"
        });
    }

    void TestGroup1() {
        CASE(N = 1);
        CASE(N = 5);
    }
};
