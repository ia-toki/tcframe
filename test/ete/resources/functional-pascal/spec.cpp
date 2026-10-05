#include <tcframe/spec.hpp>

using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    int res;

    void InputFormat() {
        LINE(N);
    }

    void OutputFormat() {
        LINE(res);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void StyleConfig() {
        FunctionalEvaluator({"encoder", "decoder"});
    }

    void Constraints() {
        CONS(1 <= N && N <= 1000);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void TestCases() {
        CASE(N = 1);
        CASE(N = 7);
        CASE(N = 1000);
    }
};
