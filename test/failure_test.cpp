#include "gtest/gtest.h"

#include "tcframe/failure.hpp"

using tcframe::Constraint;
using tcframe::FailureType;
using tcframe::SubtaskSatisfiedButNotAssignedFailure;
using tcframe::SubtaskUnsatisfiedButAssignedFailure;

TEST(SubtaskSatisfiedButNotAssignedFailureTest, Construction) {
    SubtaskSatisfiedButNotAssignedFailure failure(1);

    EXPECT_EQ(FailureType::SUBTASK_SATISFIED_BUT_NOT_ASSIGNED, failure.getType());
    EXPECT_EQ(1, failure.getSubtaskId());
}

TEST(SubtaskUnsatisfiedButAssignedFailureTest, Construction) {
    vector<Constraint*> unsatisfiedConstraints = {new Constraint([] { return true; }, "true")};
    SubtaskUnsatisfiedButAssignedFailure failure(1, unsatisfiedConstraints);

    EXPECT_EQ(FailureType::SUBTASK_UNSATISFIED_BUT_ASSIGNED, failure.getType());
    EXPECT_EQ(1, failure.getSubtaskId());
    EXPECT_EQ(unsatisfiedConstraints, failure.getUnsatisfiedConstraints());
}
