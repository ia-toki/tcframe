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
    char* argv[1] = {toChar("./runner")};

    Runner<FakeProblem> runner = Runner<FakeProblem>(argc, argv);

private:
    static char* toChar(const string& str) {
        char* cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        return cstr;
    }
};

TEST_F(RunnerTests, CompilationSuccessful) {
    runner.setGenerator(new FakeGenerator());
}

}
