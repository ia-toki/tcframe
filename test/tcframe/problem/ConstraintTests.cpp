#include "gtest/gtest.h"

#include "tcframe/problem/Constraint.hpp"

using tcframe::Constraint;

TEST(ConstraintTest, Construction) {
    int X;
    Constraint constraint([&] { return X > 7; }, "X > 7");

    EXPECT_EQ("X > 7", constraint.getDescription());

    X = 42;
    EXPECT_TRUE(constraint.isSatisfied());

    X = 1;
    EXPECT_FALSE(constraint.isSatisfied());
}
