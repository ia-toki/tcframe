#include "gtest/gtest.h"

#include "tcframe_test_commons.cpp"
#include "tcframe/runner.hpp"

using tcframe::Runner;

class MyProblem : public BaseProblem {
protected:
    void Config() {
        setSlug("foo");
    }

    void InputFormat() { }

    void OutputFormat() { }
};

class MyGenerator : public BaseGenerator<MyProblem> {
    void Config() {
        setTestCasesDir("testdata");
        setSolutionCommand("java Solution");
    }
};

TEST(RunnerTest, DefaultOptions) {
    char* argv[] = {(char*)"./runner"};

    BaseGenerator<DefaultProblem>* generator = new DefaultGenerator();

    Runner<DefaultProblem> runner(1, argv);
    runner.setGenerator(generator);

    generator->applyConfiguration();
    runner.applyCommandLineOptions();

    EXPECT_EQ("problem", generator->getSlug());
    EXPECT_EQ("tc", generator->getTestCasesDir());
    EXPECT_EQ("./solution", generator->getSolutionCommand());
}

TEST(RunnerTest, MyOptions) {
    char* argv[] = {(char*)"./runner"};

    BaseGenerator<MyProblem>* generator = new MyGenerator();

    Runner<MyProblem> runner(1, argv);
    runner.setGenerator(generator);

    generator->applyConfiguration();
    runner.applyCommandLineOptions();

    EXPECT_EQ("foo", generator->getSlug());
    EXPECT_EQ("testdata", generator->getTestCasesDir());
    EXPECT_EQ("java Solution", generator->getSolutionCommand());
}

TEST(RunnerTest, CommandLineOptions) {
    char* argv[] = {(char*)"./runner", (char*)"--slug=bar", (char*)"--tc-dir=testcases", (char*)"--solution-command=\"python sol.py\""};

    BaseGenerator<MyProblem>* generator = new MyGenerator();

    Runner<MyProblem> runner(4, argv);
    runner.setGenerator(generator);

    generator->applyConfiguration();
    runner.applyCommandLineOptions();

    EXPECT_EQ("bar", generator->getSlug());
    EXPECT_EQ("testcases", generator->getTestCasesDir());
    EXPECT_EQ("\"python sol.py\"", generator->getSolutionCommand());
}
