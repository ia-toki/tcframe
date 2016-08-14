#include "gmock/gmock.h"

#include "tcframe/spec/constraint/ConstraintSuite.hpp"

using ::testing::ElementsAre;
using ::testing::Test;

namespace tcframe {

class ConstraintSuiteBuilderTests : public Test {
protected:
    ConstraintSuiteBuilder builder;
};

TEST_F(ConstraintSuiteBuilderTests, Building) {
    ConstraintSuite constraintSuite = builder
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .build();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(-1, {
                    Constraint([]{return true;}, "1 <= A && A <= 10"),
                    Constraint([]{return true;}, "1 <= B && B <= 10")})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithSubtasks) {
    ConstraintSuite constraintSuite = builder
            .newSubtask()
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .newSubtask()
            .addConstraint([]{return true;}, "1 <= C && C <= 10")
            .addConstraint([]{return true;}, "1 <= D && D <= 10")
            .build();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(1, {
                    Constraint([]{return true;}, "1 <= A && A <= 10"),
                    Constraint([]{return true;}, "1 <= B && B <= 10")}),
            Subtask(2, {
                    Constraint([]{return true;}, "1 <= C && C <= 10"),
                    Constraint([]{return true;}, "1 <= D && D <= 10")})));
}

TEST_F(ConstraintSuiteBuilderTests, Building_WithSubtasks_WithoutLastSubtask) {
    ConstraintSuite constraintSuite = builder
            .newSubtask()
            .addConstraint([]{return true;}, "1 <= A && A <= 10")
            .addConstraint([]{return true;}, "1 <= B && B <= 10")
            .newSubtask()
            .addConstraint([]{return true;}, "1 <= C && C <= 10")
            .addConstraint([]{return true;}, "1 <= D && D <= 10")
            .newSubtask()
            .buildWithoutLastSubtask();

    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            Subtask(1, {
                    Constraint([]{return true;}, "1 <= A && A <= 10"),
                    Constraint([]{return true;}, "1 <= B && B <= 10")}),
            Subtask(2, {
                    Constraint([]{return true;}, "1 <= C && C <= 10"),
                    Constraint([]{return true;}, "1 <= D && D <= 10")})));
}

}
