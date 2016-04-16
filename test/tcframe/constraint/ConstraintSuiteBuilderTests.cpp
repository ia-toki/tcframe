#include "gmock/gmock.h"

#include "tcframe/constraint/Constraint.hpp"
#include "tcframe/constraint/ConstraintGroup.hpp"
#include "tcframe/constraint/ConstraintSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

using tcframe::Constraint;
using tcframe::ConstraintGroup;
using tcframe::ConstraintSuite;
using tcframe::ConstraintSuiteBuilder;

namespace tcframe {

class ConstraintSuiteBuilderTests : public Test {
protected:
    const Constraint constraint1 = Constraint([]{return true;}, "1 <= A && A <= 10");
    const Constraint constraint2 = Constraint([]{return true;}, "1 <= B && B <= 10");
    const Constraint constraint3 = Constraint([]{return true;}, "1 <= C && C <= 10");
    const Constraint constraint4 = Constraint([]{return true;}, "1 <= D && D <= 10");

    ConstraintSuiteBuilder builder;
};

TEST_F(ConstraintSuiteBuilderTests, CanBuildWithoutConstraintGroups) {
    ConstraintSuite constraintSuite = builder
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .build();

    EXPECT_THAT(constraintSuite.individualConstraints(), ElementsAre(
            ConstraintGroup(-1, {constraint1, constraint2})));
}

TEST_F(ConstraintSuiteBuilderTests, CanBuildWithConstraintGroups) {
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

}
