#include "gmock/gmock.h"

#include "tcframe/verifier/Verifier.hpp"

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

class VerifierTests : public Test {
protected:
    ConstraintSuite constraintSuite = ConstraintSuiteBuilder()
            .addConstraint([=]{return b1;}, "1 <= A && A <= 10")
            .addConstraint([=]{return b2;}, "1 <= B && B <= 10")
            .build();
    ConstraintSuite constraintSuiteWithSubtasks = ConstraintSuiteBuilder()
            .newSubtask()
            .addConstraint([=]{return b1;}, "1 <= A && A <= 10")
            .addConstraint([=]{return b2;}, "1 <= B && B <= 10")
            .newSubtask()
            .addConstraint([=]{return b3;}, "1 <= C && C <= 10")
            .addConstraint([=]{return b4;}, "1 <= D && D <= 10")
            .newSubtask()
            .addConstraint([=]{return b5;}, "1 <= E && E <= 10")
            .build();

    Verifier verifier = Verifier(constraintSuite);
    Verifier verifierWithSubtasks = Verifier(constraintSuiteWithSubtasks);

    void SetUp() {
        b1 = true;
        b2 = true;
        b3 = true;
        b4 = true;
        b5 = true;
    }
};

TEST_F(VerifierTests, Verification_Valid_AllConstraintsValid) {
    ConstraintsVerificationResult result = verifier.verifyConstraints({-1});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedSubtaskIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsBySubtaskId(), IsEmpty());
}

TEST_F(VerifierTests, Verification_Invalid_SomeConstraintsInvalid) {
    b2 = false;
    ConstraintsVerificationResult result = verifier.verifyConstraints({-1});

    EXPECT_FALSE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedSubtaskIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsBySubtaskId(), ElementsAre(
            Pair(-1, ElementsAre("1 <= B && B <= 10"))));
}

TEST_F(VerifierTests, Verification_WithSubtasks_Valid_AllConstraintsValid) {
    ConstraintsVerificationResult result = verifierWithSubtasks.verifyConstraints({1, 2, 3});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedSubtaskIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsBySubtaskId(), IsEmpty());
}

TEST_F(VerifierTests, Verification_WithSubtasks_Valid_AllAssignedSubtasksValid) {
    b4 = false;
    ConstraintsVerificationResult result = verifierWithSubtasks.verifyConstraints({1, 3});

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedSubtaskIds(), IsEmpty());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsBySubtaskId(), IsEmpty());
}

TEST_F(VerifierTests, Verification_WithSubtasks_Invalid_SomeConstraintsInvalid) {
    b4 = false;
    ConstraintsVerificationResult result = verifierWithSubtasks.verifyConstraints({2, 3});

    EXPECT_FALSE(result.isValid());
    EXPECT_THAT(result.satisfiedButNotAssignedSubtaskIds(), ElementsAre(1));
    EXPECT_THAT(result.unsatisfiedConstraintDescriptionsBySubtaskId(), ElementsAre(
            Pair(2, ElementsAre("1 <= D && D <= 10"))));
}

}
