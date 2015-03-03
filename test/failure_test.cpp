#include "gtest/gtest.h"

#include "tcframe/failure.hpp"

#include <vector>

using std::vector;
using tcframe::Constraint;
using tcframe::SubtaskSatisfiedButNotAssigned;
using tcframe::SubtaskUnsatisfiedButAssigned;

TEST(SubtaskFailureTest, SubtaskSatisfiedButNotAssignedConstruction) {
    SubtaskSatisfiedButNotAssigned unsatisfiability(1);

    EXPECT_EQ(1, unsatisfiability.getSubtaskId());
}

TEST(SubtaskFailureTest, SubtaskUnsatisfiedButAssignedConstruction) {
    vector<Constraint*> unsatisfiedConstraints = {new Constraint([] { return true; }, "true")};
    SubtaskUnsatisfiedButAssigned unsatisfiability(1, unsatisfiedConstraints);

    EXPECT_EQ(1, unsatisfiability.getSubtaskId());
    EXPECT_EQ(unsatisfiedConstraints, unsatisfiability.getUnsatisfiedConstraints());
}

