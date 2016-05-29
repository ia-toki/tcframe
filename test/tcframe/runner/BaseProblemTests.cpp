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

class BaseProblemTests : public Test {
protected:
    class Problem : public BaseProblem {
    protected:
        int A, B;
        vector<int> X;

        void Config() {
            setSlug("ab");
        }

        void InputFormat() {
            newLineIOSegment()
                    .addScalarVariable(Scalar::create(A, "A"))
                    .addScalarVariable(Scalar::create(B, "B"));
            newLinesIOSegment()
                    .addVectorVariable(Vector::create(X, "X"))
                    .setSize(3);
        }
    };

    class ProblemWithoutConstraintGroups : public Problem {
    protected:
        void Constraints() {
            addConstraint(Constraint([=] {return 1 <= A && A <= 100;}, "1 <= A && A <= 100"));
            addConstraint(Constraint([=] {return 1 <= B && B <= 100;}, "1 <= B && B <= 100"));
        }
    };

    class ProblemWithConstraintGroups : public Problem {
    protected:
        void Subtask1() {
            addConstraint(Constraint([=] {return 1 <= A && A <= 100;}, "1 <= A && A <= 100"));
            addConstraint(Constraint([=] {return 1 <= B && B <= 100;}, "1 <= B && B <= 100"));
            addConstraint(Constraint([=] {return A != B;}, "A != B"));
        }

        void Subtask2() {
            addConstraint(Constraint([=] {return 1 <= A && A <= 1000;}, "1 <= A && A <= 1000"));
            addConstraint(Constraint([=] {return 1 <= B && B <= 1000;}, "1 <= B && B <= 1000"));
        }
    };
    
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
        A<LineIOSegment*>(),
        A<LinesIOSegment*>()));
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
