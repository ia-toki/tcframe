#include "gmock/gmock.h"

#include "tcframe/spec/constraint/ConstraintSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class ConstraintSuiteBuilderTests : public Test {
protected:
    Constraint constraint1 = Constraint([]{return true;}, "1 <= A && A <= 10");
    Constraint constraint2 = Constraint([]{return true;}, "1 <= B && B <= 10");
    Constraint constraint3 = Constraint([]{return true;}, "1 <= C && C <= 10");
    Constraint constraint4 = Constraint([]{return true;}, "1 <= D && D <= 10");

    ConstraintSuiteBuilder builder;
};

TEST_F(ConstraintSuiteBuilderTests, Building) {
    ConstraintSuite constraintSuite = builder
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .build();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(-1, {constraint1, constraint2})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithSubtasks) {
    ConstraintSuite constraintSuite = builder
            .newSubtask()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newSubtask()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .build();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(1, {constraint1, constraint2}),
            Subtask(2, {constraint3, constraint4})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithSubtasks_WithoutLastSubtask) {
    ConstraintSuite constraintSuite = builder
            .newSubtask()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newSubtask()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .newSubtask()
            .buildWithoutLastSubtask();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(1, {constraint1, constraint2}),
            Subtask(2, {constraint3, constraint4})));
}

}
