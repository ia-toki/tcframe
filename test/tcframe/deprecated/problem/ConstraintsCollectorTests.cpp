#include "gtest/gtest.h"

#include "tcframe/deprecated/problem/ConstraintsCollector.hpp"
#include "tcframe/deprecated/problem/Subtask.hpp"

#include <vector>

using std::vector;

using tcframe::Constraint;
using tcframe::ConstraintsCollector;
using tcframe::Subtask;

TEST(DeprecatedConstraintsCollectorTest, WithSubtasks) {
    ConstraintsCollector collector;

    collector.newSubtask();
    collector.addConstraint([] { return 1 + 1 == 2; }, "1 + 1 == 2");

    collector.newSubtask();
    collector.addConstraint([] { return 1 < 2; }, "1 < 2");
    collector.addConstraint([] { return 3 == 3; }, "3 == 3");

    vector<Subtask*> subtasks = collector.collectSubtasks();

    ASSERT_EQ(2, subtasks.size());

    EXPECT_EQ(1, subtasks[0]->getId());
    EXPECT_EQ(1, subtasks[0]->getConstraints().size());

    EXPECT_EQ(2, subtasks[1]->getId());
    EXPECT_EQ(2, subtasks[1]->getConstraints().size());
}

TEST(DeprecatedConstraintsCollectorTest, WithoutSubtasks) {
    ConstraintsCollector collector;

    collector.addConstraint([] { return 1 + 1 == 2; }, "1 + 1 == 2");
    collector.addConstraint([] { return 1 < 2; }, "1 < 2");
    collector.addConstraint([] { return 3 == 3; }, "3 == 3");

    vector<Subtask*> subtasks = collector.collectSubtasks();

    ASSERT_EQ(1, subtasks.size());

    EXPECT_EQ(-1, subtasks[0]->getId());
    EXPECT_EQ(3, subtasks[0]->getConstraints().size());
}
