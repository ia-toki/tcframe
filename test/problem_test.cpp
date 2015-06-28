#include "gtest/gtest.h"

#include "tcframe_test_commons.cpp"
#include "tcframe/problem.hpp"

using tcframe::BaseProblem;

class MyProblem : public BaseProblem {
protected:
    void Config() {
        setSlug("foo");
        setTimeLimit(2);
        setMemoryLimit(256);
    }

    void InputFormat() { }

    void OutputFormat() { }
};

TEST(ProblemTest, DefaultOptions) {
    BaseProblem* problem = new DefaultProblem();

    EXPECT_EQ("problem", problem->getSlug());
    EXPECT_EQ(0, problem->getTimeLimit());
    EXPECT_EQ(0, problem->getMemoryLimit());
}

TEST(ProblemTest, MyOptions) {
    BaseProblem* problem = new MyProblem();
    problem->applyProblemConfiguration();

    EXPECT_EQ("foo", problem->getSlug());
    EXPECT_EQ(2, problem->getTimeLimit());
    EXPECT_EQ(256, problem->getMemoryLimit());
}
