#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using ::testing::A;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Property;
using ::testing::SizeIs;
using ::testing::Test;

namespace tcframe {

class Problem : public BaseProblem {
protected:
    int A, B;

    void Config() {
        setSlug("ab");
    }

    void InputFormat() {
        LINE(A, B);
    }
};

class ProblemWithoutConstraintGroups : public Problem {
protected:
    void Constraints() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
    }
};

class ProblemWithConstraintGroups : public Problem {
protected:
    void Subtask1() {
        CONS(1 <= A && A <= 100);
        CONS(1 <= B && B <= 100);
        CONS(A != B);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
};

class BaseProblemTests : public Test {
protected:
    Problem problem;
    ProblemWithoutConstraintGroups problemWithoutConstraintGroups;
    ProblemWithConstraintGroups problemWithConstraintGroups;
};

TEST_F(BaseProblemTests, Config) {
    ProblemConfig config = problem.buildProblemConfig();
    EXPECT_THAT(config.slug(), Eq("ab"));
}

TEST_F(BaseProblemTests, IOFormat) {
    IOFormat ioFormat = problem.buildIOFormat();
    EXPECT_THAT(ioFormat.inputFormat(), ElementsAre(
        A<LineIOSegment*>()));
}

TEST_F(BaseProblemTests, Constraints) {
    ConstraintSuite constraintSuite = problemWithoutConstraintGroups.buildConstraintSuite();
    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
        AllOf(Property(&ConstraintGroup::id, -1), Property(&ConstraintGroup::constraints, SizeIs(2)))));
}

TEST_F(BaseProblemTests, Subtasks) {
    ConstraintSuite constraintSuite = problemWithConstraintGroups.buildConstraintSuite();
    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
            AllOf(Property(&ConstraintGroup::id, 1), Property(&ConstraintGroup::constraints, SizeIs(3))),
            AllOf(Property(&ConstraintGroup::id, 2), Property(&ConstraintGroup::constraints, SizeIs(2)))));
}

}
