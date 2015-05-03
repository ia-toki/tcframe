#include "gtest/gtest.h"

#include "tcframe/runner.hpp"

using tcframe::BaseGenerator;
using tcframe::BaseProblem;
using tcframe::Runner;

class DefaultProblem : public BaseProblem {
protected:
    void InputFormat() { }

    void OutputFormat() { }
};

class DefaultGenerator : public BaseGenerator<DefaultProblem> {

};

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
    Runner<DefaultProblem> runner(1, argv);

    BaseGenerator<DefaultProblem>* generator = new DefaultGenerator();
    runner.setGenerator(generator);

    generator->applyConfigurations();
    runner.applyCommandLineOptions();

    EXPECT_EQ("problem", generator->getProblemSlug());
    EXPECT_EQ("tc", generator->getTestCasesDir());
    EXPECT_EQ("./solution", generator->getSolutionCommand());
}

TEST(RunnerTest, MyOptions) {
    char* argv[] = {(char*)"./runner"};
    Runner<MyProblem> runner(1, argv);

    BaseGenerator<MyProblem>* generator = new MyGenerator();
    runner.setGenerator(generator);

    generator->applyConfigurations();
    runner.applyCommandLineOptions();

    EXPECT_EQ("foo", generator->getProblemSlug());
    EXPECT_EQ("testdata", generator->getTestCasesDir());
    EXPECT_EQ("java Solution", generator->getSolutionCommand());
}

TEST(RunnerTest, CommandLineOptions) {
    char* argv[] = {(char*)"./runner", (char*)"--slug=bar", (char*)"--tc-dir=testcases", (char*)"--solution-command=\"python sol.py\""};
    Runner<MyProblem> runner(4, argv);

    BaseGenerator<MyProblem>* generator = new MyGenerator();
    runner.setGenerator(generator);

    generator->applyConfigurations();
    runner.applyCommandLineOptions();

    EXPECT_EQ("bar", generator->getProblemSlug());
    EXPECT_EQ("testcases", generator->getTestCasesDir());
    EXPECT_EQ("\"python sol.py\"", generator->getSolutionCommand());
}