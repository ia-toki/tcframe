#include "gmock/gmock.h"

#include "tcframe/constraint/ConstraintSuite.hpp"

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

    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
            ConstraintGroup(-1, {constraint1, constraint2})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithGroups) {
    ConstraintSuite constraintSuite = builder
            .newConstraintGroup()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newConstraintGroup()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .build();

    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
            ConstraintGroup(1, {constraint1, constraint2}),
            ConstraintGroup(2, {constraint3, constraint4})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithGroups_WithoutLastGroup) {
    ConstraintSuite constraintSuite = builder
            .newConstraintGroup()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newConstraintGroup()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .newConstraintGroup()
            .buildWithoutLastConstraintGroup();

    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
            ConstraintGroup(1, {constraint1, constraint2}),
            ConstraintGroup(2, {constraint3, constraint4})));
}

}
