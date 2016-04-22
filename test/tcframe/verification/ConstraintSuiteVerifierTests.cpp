#include "gmock/gmock.h"

#include "tcframe/verification/ConstraintSuiteVerifier.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Pair;
using ::testing::Test;

namespace tcframe {

bool b1;
bool b2;
bool b3;
bool b4;
bool b5;

class ConstraintSuiteVerifierTests : public Test {
protected:
    const Constraint constraint1 = Constraint([=]{return b1;}, "1 <= A && A <= 10");
    const Constraint constraint2 = Constraint([=]{return b2;}, "1 <= B && B <= 10");
    const Constraint constraint3 = Constraint([=]{return b3;}, "1 <= C && C <= 10");
    const Constraint constraint4 = Constraint([=]{return b4;}, "1 <= D && D <= 10");
    const Constraint constraint5 = Constraint([=]{return b5;}, "1 <= E && E <= 10");

    const ConstraintSuite constraintSuiteWithoutConstraintGroups = ConstraintSuiteBuilder()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .build();

    const ConstraintSuite constraintSuiteWithConstraintGroups = ConstraintSuiteBuilder()
            .newConstraintGroup()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newConstraintGroup()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .newConstraintGroup()
            .addConstraint(constraint5)
            .build();
};

TEST_F(ConstraintSuiteVerifierTests, VerificationWithoutConstraintGroupsValidWhenAllConstraintsValid) {
    ConstraintSuiteVerifier verifier(constraintSuiteWithoutConstraintGroups);

    b1 = true;
    b2 = true;
    ConstraintSuiteVerificationResult result = verifier.verify({-1});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedConstraintGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, VerificationWithoutConstraintGroupsInvalidWhenSomeConstraintsInvalid) {
    ConstraintSuiteVerifier verifier(constraintSuiteWithoutConstraintGroups);

    b1 = true;
    b2 = false;
    ConstraintSuiteVerificationResult result = verifier.verify({-1});

    EXPECT_THAT(result.isValid(), Eq(false));
    EXPECT_THAT(result.satisfiedButNotAssignedConstraintGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(-1, ElementsAre(constraint2.description()))));
}


TEST_F(ConstraintSuiteVerifierTests, VerificationWithConstraintGroupsValidWhenAllConstraintsValid) {
    ConstraintSuiteVerifier verifier(constraintSuiteWithConstraintGroups);

    b1 = true;
    b2 = true;
    b3 = true;
    b4 = true;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifier.verify({1, 2, 3});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedConstraintGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}


TEST_F(ConstraintSuiteVerifierTests, VerificationWithConstraintGroupsValidWhenAllAssignedConstraintGroupsValid) {
    ConstraintSuiteVerifier verifier(constraintSuiteWithConstraintGroups);

    b1 = true;
    b2 = true;
    b3 = true;
    b4 = false;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifier.verify({1, 3});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedConstraintGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, VerificationWithConstraintGroupsInvalidWhenSomeConstraintsInvalid) {
    ConstraintSuiteVerifier verifier(constraintSuiteWithConstraintGroups);

    b1 = true;
    b2 = true;
    b3 = true;
    b4 = false;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifier.verify({2, 3});

    EXPECT_THAT(result.isValid(), Eq(false));
    EXPECT_THAT(result.satisfiedButNotAssignedConstraintGroupIds(), ElementsAre(1));
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(2, ElementsAre(constraint4.description()))));
}

}
