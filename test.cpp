#include "include/cptest/cptest.h"

#include <string>

using namespace org::iatoki::cptest;
using std::string;

struct Problem : BaseProblem {
    int A, B;

    void inputFormat() {
        line() % A % B;
    }

    void subtask1() {
        addPredicate(Predicate([this] {return A % 2 == 0;}, "A % 2 == 0"));
        addPredicate(Predicate([this] {return B == 4;}, "B == 4"));
    }
};

struct Generator : BaseGenerator<Problem> {
    void testSet1() {
        assignToSubtasks({0});

        addTestCase(TestCase([this] { A = 3; B = 4;}, ""));
        addTestCase(TestCase([this] { A = 7; B = 4;}, ""));
    }

    void testSet2() {
        assignToSubtasks({0});

        addTestCase(TestCase([this] { A = 1; B = 1;}, ""));
        addTestCase(TestCase([this] { A = 2; B = 2;}, ""));
    }
};

int main() {

    Generator gen;
    gen.generate();
}