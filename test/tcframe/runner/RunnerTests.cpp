#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using ::testing::Test;

namespace tcframe {

class RunnerTests : public Test {
protected:
    class FakeProblem : public BaseProblem {
    protected:
        void InputFormat() {}
    };

    class FakeGenerator : public BaseGenerator<FakeProblem> {};
};

TEST_F(RunnerTests, CompilationSuccessful) {
    int argc = 1;
    char* argv[1] = {(char*) "./runner"};
    Runner<FakeProblem> runner = Runner<FakeProblem>(argc, argv);
    runner.setGenerator(new FakeGenerator());
}

}
