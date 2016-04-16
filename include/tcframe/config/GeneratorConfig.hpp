#pragma once

#include <string>
#include <utility>

using std::move;
using std::string;

namespace tcframe { namespace experimental {

struct GeneratorConfig {
    friend class GeneratorConfigBuilder;

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
};

class GeneratorConfigBuilder {
private:
    GeneratorConfig subject_;

public:
    GeneratorConfigBuilder() {
        subject_.solutionCommand_ = "./solution";
        subject_.testCasesDir_ = "tc";
    }

    GeneratorConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    GeneratorConfigBuilder& setTestCasesDir(string testCasesDir) {
        subject_.testCasesDir_ = testCasesDir;
        return *this;
    }

    GeneratorConfig build() {
        return move(subject_);
    }
};

}}
