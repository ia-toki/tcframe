#include "gmock/gmock.h"

#include "tcframe/spec/constraint/ConstraintSuite.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class ConstraintSuiteBuilderTests : public Test {
protected:
    ConstraintSuiteBuilder builder;
    ConstraintSuiteBuilder builder1;
    ConstraintSuiteBuilder builder2;
};

TEST_F(ConstraintSuiteBuilderTests, Building_Nothing) {
    ConstraintSuite constraintSuite = builder
            .build();
    ConstraintSuite expected({Subtask(Subtask::MAIN_ID, Subtask::MAIN_POINTS, {})}, {});

    EXPECT_THAT(constraintSuite, Eq(expected));
}

TEST_F(ConstraintSuiteBuilderTests, Building_OnlyConstraints) {
    ConstraintSuite constraintSuite = builder
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .build();
    ConstraintSuite expected({Subtask(Subtask::MAIN_ID, Subtask::MAIN_POINTS, {
            Constraint([]{return true;}, "1 <= A && A <= 10"),
            Constraint([]{return true;}, "1 <= B && B <= 10")})}, {});

    EXPECT_THAT(constraintSuite, Eq(expected));
}

TEST_F(ConstraintSuiteBuilderTests, Building_OnlyMultipleTestCasesConstraints) {
    ConstraintSuite constraintSuite = builder
            .prepareForMultipleTestCasesConstraints()
            .addConstraint([]{return true;}, "1 <= T && T <= 10")
            .build();
    ConstraintSuite expected({Subtask(Subtask::MAIN_ID, Subtask::MAIN_POINTS, {})}, {
            Constraint([]{return true;}, "1 <= T && T <= 10")});

    EXPECT_THAT(constraintSuite, Eq(expected));
}

TEST_F(ConstraintSuiteBuilderTests, Building_Both) {
    ConstraintSuite constraintSuite = builder
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .prepareForMultipleTestCasesConstraints()
            .addConstraint([]{return true;}, "1 <= T && T <= 10")
            .build();
    ConstraintSuite expected({Subtask(Subtask::MAIN_ID, Subtask::MAIN_POINTS, {
            Constraint([]{return true;}, "1 <= A && A <= 10"),
            Constraint([]{return true;}, "1 <= B && B <= 10")})}, {
            Constraint([]{return true;}, "1 <= T && T <= 10")});

    EXPECT_THAT(constraintSuite, Eq(expected));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithSubtasks) {
    ConstraintSuite constraintSuite1 = builder1
            .newSubtask()
            .Points(70)
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .newSubtask()
            .Points(30)
            .addConstraint([]{return true;}, "1 <= C && C <= 10")
            .addConstraint([]{return true;}, "1 <= D && D <= 10")
            .build();
    ConstraintSuite constraintSuite2 = builder2
            .newSubtask()
            .Points(70)
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .newSubtask()
            .Points(30)
            .addConstraint([]{return true;}, "1 <= C && C <= 10")
            .addConstraint([]{return true;}, "1 <= D && D <= 10")
            .newSubtask() // should be ignored
            .build();
    ConstraintSuite expected({
            Subtask(Subtask::MAIN_ID, {}),
            Subtask(1, 70, {
                    Constraint([]{return true;}, "1 <= A && A <= 10"),
                    Constraint([]{return true;}, "1 <= B && B <= 10")}),
            Subtask(2, 30, {
                    Constraint([]{return true;}, "1 <= C && C <= 10"),
                    Constraint([]{return true;}, "1 <= D && D <= 10")})}, {});

    EXPECT_THAT(constraintSuite1, Eq(expected));
    EXPECT_THAT(constraintSuite2, Eq(expected));
}

TEST_F(ConstraintSuiteBuilderTests, Building_GlobalConstraintsAndSubtasks) {
    ConstraintSuite constraintSuite = builder
            .addConstraint([]{return true;}, "1 <= X && X <= 100")
            .addConstraint([]{return true;}, "1 <= Y && Y <= 100")
            .newSubtask()
            .Points(70)
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .newSubtask()
            .addConstraint([]{return true;}, "1 <= C && C <= 10")
            .addConstraint([]{return true;}, "1 <= D && D <= 10")
            .build();
    ConstraintSuite expected({
             Subtask(Subtask::MAIN_ID, {
                     Constraint([]{return true;}, "1 <= X && X <= 100"),
                     Constraint([]{return true;}, "1 <= Y && Y <= 100")}),
             Subtask(1, 70, {
                     Constraint([]{return true;}, "1 <= A && A <= 10"),
                     Constraint([]{return true;}, "1 <= B && B <= 10")}),
             Subtask(2, {
                     Constraint([]{return true;}, "1 <= C && C <= 10"),
                     Constraint([]{return true;}, "1 <= D && D <= 10")})}, {});

    EXPECT_THAT(constraintSuite, Eq(expected));
}

}
