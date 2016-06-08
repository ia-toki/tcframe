#pragma once

#include <string>
#include <tuple>
#include <utility>

using std::move;
using std::string;
using std::tuple;

namespace tcframe {

struct TestConfig {
    friend class TestConfigBuilder;

private:
    string solutionCommand_;
    string testCasesDir_;

public:
    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const string& testCasesDir() const {
        return testCasesDir_;
    }

    bool operator==(const TestConfig& o) const {
        return tie(solutionCommand_, testCasesDir_) == tie(o.solutionCommand_, o.testCasesDir_);
    }
};

class TestConfigBuilder {
private:
    TestConfig subject_;

public:
    virtual ~TestConfigBuilder() {}

    TestConfigBuilder() {
        subject_.solutionCommand_ = "./solution";
        subject_.testCasesDir_ = "tc";
    }

    TestConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    TestConfigBuilder& setTestCasesDir(string testCasesDir) {
        subject_.testCasesDir_ = testCasesDir;
        return *this;
    }

    TestConfig build() {
        return move(subject_);
    }
};

}
