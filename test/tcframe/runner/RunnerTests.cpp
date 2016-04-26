#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using ::testing::Test;

namespace tcframe {

class FakeProblem : public BaseProblem {
protected:
    void InputFormat() {}
};

class FakeGenerator : public BaseGenerator<FakeProblem> {};

class RunnerTests : public Test {
protected:
    int argc = 1;
    char* argv[1] = {(char*) "./runner"};

    Runner<FakeProblem> runner = Runner<FakeProblem>(argc, argv);
};

TEST_F(RunnerTests, CompilationSuccessful) {
    FakeGenerator generator;
    runner.setGenerator(&generator);
}

}
