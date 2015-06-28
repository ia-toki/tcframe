#include "gtest/gtest.h"

#include "tcframe_test_commons.cpp"

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

TEST(ProblemTest, CommandLineOptions) {
    char* argv[] = {(char*)"<runner>", (char*)"--slug=bar", (char*)"--time-limit=3", (char*)"--memory-limit=64"};

    BaseProblem* problem = new MyProblem();
    problem->applyProblemConfiguration();
    problem->applyProblemCommandLineOptions(4, argv);

    EXPECT_EQ("bar", problem->getSlug());
    EXPECT_EQ(3, problem->getTimeLimit());
    EXPECT_EQ(64, problem->getMemoryLimit());
}


TEST(ProblemTest, CommandLineOptionsWithNos) {
    char* argv[] = {(char*)"<runner>", (char*)"--no-time-limit", (char*)"--no-memory-limit"};

    BaseProblem* problem = new MyProblem();
    problem->applyProblemConfiguration();
    problem->applyProblemCommandLineOptions(3, argv);

    EXPECT_EQ("foo", problem->getSlug());
    EXPECT_EQ(0, problem->getTimeLimit());
    EXPECT_EQ(0, problem->getMemoryLimit());
}
