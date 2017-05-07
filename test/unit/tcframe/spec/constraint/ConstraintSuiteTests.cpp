#include "gmock/gmock.h"

#include "tcframe/spec/constraint/ConstraintSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class ConstraintSuiteTests : public Test {};

TEST_F(ConstraintSuiteTests, HasSubtasks_WithoutSubtasks) {
    ConstraintSuite constraintSuite({Subtask(Subtask::MAIN_ID, {})}, {});
    EXPECT_FALSE(constraintSuite.hasSubtasks());
}

TEST_F(ConstraintSuiteTests, HasSubtasks_WithSubtasks) {
    ConstraintSuite constraintSuite({Subtask(Subtask::MAIN_ID, {}), Subtask(1, {})}, {});
    EXPECT_TRUE(constraintSuite.hasSubtasks());
}

TEST_F(ConstraintSuiteTests, GetDefinedSubtaskIds) {
    ConstraintSuite constraintSuite({Subtask(Subtask::MAIN_ID, {}), Subtask(1, {}), Subtask(2, {})}, {});
    EXPECT_THAT(constraintSuite.getDefinedSubtaskIds(), ElementsAre(1, 2));
}

}
