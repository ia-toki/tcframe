#include "tcframe/tcframe.h"

using namespace tcframe;

struct Problem : BaseProblem {
    int A, B, K;

    void Config() {
        setSlug("example");
    }

    void InputFormat() {
        line() % A, B;
        line() % K;
    }

    void Subtask1() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
        CONS(0 <= K && K <= 1);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 2000000000);
        CONS(1 <= B && B <= 2000000000);
        CONS(0 <= K && K <= 1000);
    }
};

struct Generator : BaseGenerator<Problem> {
    void Config() {
        setBaseDir("tc");
        setSolution("./solution");
    }

    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 2000, K = 2);
        CASE(A = 100, B = 200, K = 1);
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 1001, B = 10000, K = 42);
        CASE(A = 1, B = 2, K = 0);
        CASE(A = 2000000000, B = 2000000000, K = 0);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
