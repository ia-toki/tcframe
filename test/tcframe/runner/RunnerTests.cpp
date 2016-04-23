#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using namespace tcframe;

class ProblemWithoutConstraintGroups : public BaseProblem {
protected:
    int A, B;

    void Config() {
        setSlug("ab");
    }

    void InputFormat() {
        LINE(A, B);
    }

    void Constraints() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
    }
};

class GeneratorWithoutTestGroups : public BaseGenerator<ProblemWithoutConstraintGroups> {
protected:
    void Config() {
        setSolutionCommand("python Sol.py");
        setTestCasesDir("dir");
    }

    void TestCases() {
        CASE(A = 1, B = 2);
        CASE(A = 3, B = 4);
    }
};

class ProblemWithConstraintGroups : public BaseProblem {
protected:
    int A, B;

    void Config() {
        setSlug("ab");
    }

    void InputFormat() {
        LINE(A, B);
    }

    void Subtask1() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
};

TEST(RunnerTests, WithoutGroups_MagicValid) {
    Runner<ProblemWithoutConstraintGroups> runner(1, NULL);

    runner.setGenerator(new GeneratorWithoutTestGroups());
}
