#include "gmock/gmock.h"

#include "tcframe/verifier/Verifier.hpp"

using ::testing::ElementsAre;
using ::testing::IsEmpty;
using ::testing::Pair;
using ::testing::Test;

namespace tcframe {

class VerifierTests : public Test {
public:
    static bool b0, b1, b2, b3, b4, b5;

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
    ConstraintSuite constraintSuiteWithMultipleTestCasesConstraints = ConstraintSuiteBuilder()
            .addConstraint([=]{return b1;}, "1 <= A && A <= 10")
            .addConstraint([=]{return b2;}, "1 <= B && B <= 10")
            .prepareForMultipleTestCasesConstraints()
            .addConstraint([=]{return b0;}, "1 <= T && T <= 10")
            .build();

    Verifier verifier = Verifier(constraintSuite);
    Verifier verifierWithSubtasks = Verifier(constraintSuiteWithSubtasks);
    Verifier verifierWithMultipleTestCasesConstraints = Verifier(constraintSuiteWithMultipleTestCasesConstraints);

    void SetUp() {
        b0 = b1 = b2 = b3 = b4 = b5 = true;
    }
};

bool VerifierTests::b0;
bool VerifierTests::b1;
bool VerifierTests::b2;
bool VerifierTests::b3;
bool VerifierTests::b4;
bool VerifierTests::b5;

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

TEST_F(VerifierTests, Verification_MultipleTestCases_Valid_AllConstraintsValid) {
    MultipleTestCasesConstraintsVerificationResult result =
            verifierWithMultipleTestCasesConstraints.verifyMultipleTestCasesConstraints();

    EXPECT_TRUE(result.isValid());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptions(), IsEmpty());
}

TEST_F(VerifierTests, Verification_MultipleTestCases_Invalid_SomeConstraintsInvalid) {
    b0 = false;
    MultipleTestCasesConstraintsVerificationResult result =
            verifierWithMultipleTestCasesConstraints.verifyMultipleTestCasesConstraints();

    EXPECT_FALSE(result.isValid());
    EXPECT_THAT(result.unsatisfiedConstraintDescriptions(), ElementsAre(
            "1 <= T && T <= 10"));
}

}
