#include "include/cptest/cptest.h"

#include <string>

using namespace org::iatoki::cptest;
using std::string;

struct Problem : BaseProblem {
    int A, B;

    void Config() {
        setSlug("test-problem");
    }

    void InputFormat() {
        line() % A % B;
    }

    void Subtask1() {
        CONS(A % 2 == 0);
        CONS(B == 4);
    }
};

struct Generator : BaseGenerator<Problem> {
    void TestGroup1() {
        assignToSubtasks({1});

        CASE(A = 3; B = 4;);
        CASE(A = 7; B = 4;);
    }

    void TestGroup2() {
        assignToSubtasks({1});

        CASE(A = 1; B = 1;);
        CASE(A = 2; B = 2;);
    }
};

int main() {
    Generator gen;
    gen.generate();
}