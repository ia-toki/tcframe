#include "tcframe/tcframe.h"

using namespace tcframe;

class Problem : public BaseProblem {
protected:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        line() % A, B;
        line() % K;
    }

    void Subtask1() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
        CONS(0 <= K && K <= 100);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 2000000000);
        CONS(1 <= B && B <= 2000000000);
        CONS(0 <= K && K <= 10000);
    }

    void Subtask3() {
        CONS(A == 0);
        CONS(B == 0);
        CONS(0 <= K && K <= 100);
    }
};

class Generator : public BaseGenerator<Problem> {
protected:
    void Config() {
        setBaseDir("tc");
        setSolution("./solution");
    }

    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 100, K = 7);
        CASE(A = 100, B = 2000, K = 0);
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 1, B = 2, K = 1);
        CASE(A = 0, B = 0, K = 100);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
