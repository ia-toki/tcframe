#include "gtest/gtest.h"

#include "tcframe/failure.hpp"

#include <vector>

using std::vector;
using tcframe::Constraint;
using tcframe::FailureDescriptionItem;
using tcframe::IOFormatFailure;
using tcframe::SubtaskFailure;
using tcframe::SubtaskSatisfiedButNotAssigned;
using tcframe::SubtaskUnsatisfiability;
using tcframe::SubtaskUnsatisfiedButAssigned;

TEST(FailureDescriptionItemTest, Construction) {
    FailureDescriptionItem item("some text", 7);

    EXPECT_EQ("some text", item.getMessage());
    EXPECT_EQ(7, item.getLevel());
}

TEST(SubtaskSatisfiedButNotAssignedTest, Construction) {
    SubtaskSatisfiedButNotAssigned unsatisfiability(1);

    EXPECT_EQ(1, unsatisfiability.getSubtaskId());

    vector<FailureDescriptionItem> description = unsatisfiability.getDescription();

    ASSERT_EQ(1, description.size());
    EXPECT_EQ("Satisfies subtask 1 but is not assigned to it", description[0].getMessage());
    EXPECT_EQ(0, description[0].getLevel());
}

TEST(SubtaskUnsatisfiedButAssignedTest, Construction) {
    vector<Constraint*> unsatisfiedConstraints = {new Constraint([] { return true; }, "true")};
    SubtaskUnsatisfiedButAssigned unsatisfiability(1, unsatisfiedConstraints);

    EXPECT_EQ(1, unsatisfiability.getSubtaskId());
    EXPECT_EQ(unsatisfiedConstraints, unsatisfiability.getUnsatisfiedConstraints());

    vector<FailureDescriptionItem> description = unsatisfiability.getDescription();

    ASSERT_EQ(2, description.size());
    EXPECT_EQ("Does not satisfy subtask 1, on constraints:", description[0].getMessage());
    EXPECT_EQ(0, description[0].getLevel());

    EXPECT_EQ("true", description[1].getMessage());
    EXPECT_EQ(1, description[1].getLevel());
}

TEST(SubtaskUnsatisfiedButAssignedTest, ConstructionWithoutSubtasks) {
    vector<Constraint*> unsatisfiedConstraints = {new Constraint([] { return true; }, "true")};
    SubtaskUnsatisfiedButAssigned unsatisfiability(-1, unsatisfiedConstraints);

    EXPECT_EQ(-1, unsatisfiability.getSubtaskId());
    EXPECT_EQ(unsatisfiedConstraints, unsatisfiability.getUnsatisfiedConstraints());

    vector<FailureDescriptionItem> description = unsatisfiability.getDescription();

    ASSERT_EQ(2, description.size());
    EXPECT_EQ("Does not satisfy constraints, on:", description[0].getMessage());
    EXPECT_EQ(0, description[0].getLevel());

    EXPECT_EQ("true", description[1].getMessage());
    EXPECT_EQ(1, description[1].getLevel());
}

TEST(SubtaskFailureTest, Construction) {
    vector<SubtaskUnsatisfiability*> unsatisfiabilities = {nullptr, nullptr};
    SubtaskFailure* failure = new SubtaskFailure(unsatisfiabilities);

    EXPECT_EQ(unsatisfiabilities, failure->getUnsatisfiabilities());
}

TEST(IOFormatFailureTest, Construction) {
    IOFormatFailure* failure = new IOFormatFailure("some text");

    EXPECT_EQ("some text", failure->getMessage());
}