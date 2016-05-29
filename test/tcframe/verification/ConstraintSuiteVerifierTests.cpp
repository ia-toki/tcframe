#include "gmock/gmock.h"

#include "tcframe/verification/ConstraintSuiteVerifier.hpp"

using ::testing::ElementsAre;
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

    ConstraintSuiteVerifier verifier = ConstraintSuiteVerifier(constraintSuiteWithoutGroups);
    ConstraintSuiteVerifier verifierWithGroups = ConstraintSuiteVerifier(constraintSuiteWithGroups);

    void SetUp() {
        b1 = true;
        b2 = true;
        b3 = true;
        b4 = true;
        b5 = true;
    }
};

TEST_F(ConstraintSuiteVerifierTests, Verification_Valid_AllConstraintsValid) {
    ConstraintSuiteVerificationResult result = verifier.verify({-1});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, Verification_Invalid_SomeConstraintsInvalid) {
    b2 = false;
    ConstraintSuiteVerificationResult result = verifier.verify({-1});

    EXPECT_FALSE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(-1, ElementsAre(constraint2.description()))));
}

TEST_F(ConstraintSuiteVerifierTests, Verification_WithGroups_Valid_AllConstraintsValid) {
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({1, 2, 3});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, Verification_WithGroups_Valid_AllAssignedConstraintGroupsValid) {
    b4 = false;
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({1, 3});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), IsEmpty());
}

TEST_F(ConstraintSuiteVerifierTests, Verification_WithGroups_Invalid_SomeConstraintsInvalid) {
    b4 = false;
    ConstraintSuiteVerificationResult result = verifierWithGroups.verify({2, 3});

    EXPECT_FALSE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedGroupIds(), ElementsAre(1));
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsByConstraintGroupId(), ElementsAre(
            Pair(2, ElementsAre(constraint4.description()))));
}

}
