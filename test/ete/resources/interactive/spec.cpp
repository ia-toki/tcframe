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
        CONS(valueOf(N).isBetween(1, 10));
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "3"
        });
    }

    void TestCases() {
        CASE(N = 1);
        CASE(N = 2);
        CASE(N = 10);
    }
};
