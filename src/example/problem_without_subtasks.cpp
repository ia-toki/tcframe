#include "tcframe/tcframe.hpp"

using namespace tcframe;

class Problem : public BaseProblem {
protected:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        LINE(A, B);
        LINE(K);
    }

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
        CONS(0 <= K && K <= 100);
    }
};

class Generator : public BaseGenerator<Problem> {
protected:
    void Config() {
        setBaseDir("tc");
        setSolution("./solution");
    }

    void TestCases() {
        CASE(A = 1, B = 100, K = 7);
        CASE(A = 100, B = 2000, K = 0);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
