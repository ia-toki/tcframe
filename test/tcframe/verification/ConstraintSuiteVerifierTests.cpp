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
    Constraint constraint1 = Constraint([=]{return b1;}, "1 <= A && A <= 10");
    Constraint constraint2 = Constraint([=]{return b2;}, "1 <= B && B <= 10");
    Constraint constraint3 = Constraint([=]{return b3;}, "1 <= C && C <= 10");
    Constraint constraint4 = Constraint([=]{return b4;}, "1 <= D && D <= 10");
    Constraint constraint5 = Constraint([=]{return b5;}, "1 <= E && E <= 10");
    ConstraintSuite constraintSuiteWithoutGroups = ConstraintSuiteBuilder()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .build();
    ConstraintSuite constraintSuiteWithGroups = ConstraintSuiteBuilder()
            .newConstraintGroup()
            .addConstraint(constraint1)
            .addConstraint(constraint2)
            .newConstraintGroup()
            .addConstraint(constraint3)
            .addConstraint(constraint4)
            .newConstraintGroup()
            .addConstraint(constraint5)
            .build();

    ConstraintSuiteVerifier verifierWithoutGroups = ConstraintSuiteVerifier(constraintSuiteWithoutGroups);
    ConstraintSuiteVerifier verifierWithGroups = ConstraintSuiteVerifier(constraintSuiteWithGroups);
};

TEST_F(ConstraintSuiteVerifierTests, WithoutGroups_VerificationValid_WhenAllConstraintsValid) {
    b1 = true;
    b2 = true;
    ConstraintSuiteVerificationResult result = verifierWithoutGroups.verify({-1});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, WithoutGroups_VerificationInvalid_SomeConstraintsInvalid) {
    b1 = true;
    b2 = false;
    ConstraintSuiteVerificationResult result = verifierWithoutGroups.verify({-1});

    EXPECT_THAT(result.isValid(), Eq(false));
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(-1, ElementsAre(constraint2.description()))));
}

TEST_F(ConstraintSuiteVerifierTests, WithGroups_VerificationValid_AllConstraintsValid) {
    b1 = true;
    b2 = true;
    b3 = true;
    b4 = true;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({1, 2, 3});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, WithGroups_VerificationValid_AllAssignedConstraintGroupsValid) {
    b1 = true;
    b2 = true;
    b3 = true;
    b4 = false;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({1, 3});

    EXPECT_THAT(result.isValid(), Eq(true));
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, WithGroups_VerificationInvalid_SomeConstraintsInvalid) {
    b1 = true;
    b2 = true;
    b3 = true;
    b4 = false;
    b5 = true;
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({2, 3});

    EXPECT_THAT(result.isValid(), Eq(false));
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), ElementsAre(1));
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(2, ElementsAre(constraint4.description()))));
}

}
