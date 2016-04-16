#include "gtest/gtest.h"

#include "../commons/Commons.hpp"
#include "tcframe/deprecated/problem/BaseProblem.hpp"

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

TEST(DeprecatedBaseProblemTest, DefaultOptions) {
    DefaultProblem problem;

    EXPECT_EQ("problem", problem.getSlug());
    EXPECT_EQ(0, problem.getTimeLimit());
    EXPECT_EQ(0, problem.getMemoryLimit());
}

TEST(DeprecatedBaseProblemTest, MyOptions) {
    MyProblem problem;
    problem.applyProblemConfiguration();

    EXPECT_EQ("foo", problem.getSlug());
    EXPECT_EQ(2, problem.getTimeLimit());
    EXPECT_EQ(256, problem.getMemoryLimit());
}

TEST(DeprecatedBaseProblemTest, CommandLineOptions) {
    char* argv[] = {(char*)"<runner>", (char*)"--slug=bar", (char*)"--time-limit=3", (char*)"--memory-limit=64"};

    MyProblem problem;
    problem.applyProblemConfiguration();
    problem.applyProblemCommandLineOptions(4, argv);

    EXPECT_EQ("bar", problem.getSlug());
    EXPECT_EQ(3, problem.getTimeLimit());
    EXPECT_EQ(64, problem.getMemoryLimit());
}


TEST(DeprecatedBaseProblemTest, CommandLineOptionsWithNos) {
    char* argv[] = {(char*)"<runner>", (char*)"--no-time-limit", (char*)"--no-memory-limit"};

    MyProblem problem;
    problem.applyProblemConfiguration();
    problem.applyProblemCommandLineOptions(3, argv);

    EXPECT_EQ("foo", problem.getSlug());
    EXPECT_EQ(0, problem.getTimeLimit());
    EXPECT_EQ(0, problem.getMemoryLimit());
}
