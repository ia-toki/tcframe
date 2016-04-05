#pragma once

#include <algorithm>
#include <string>

using std::move;
using std::string;

namespace tcframe { namespace experimental {

struct TestSuiteConfig {
    friend class TestSuiteConfigBuilder;

private:
    string slug_;
    string testCasesDir_;
    string solutionCommand_;

public:
    const string& slug() const {
        return slug_;
    }

    const string& testCasesDir() const {
        return testCasesDir_;
    }

    const string& solutionCommand() const {
        return solutionCommand_;
    }
};

class TestSuiteConfigBuilder {
private:
    TestSuiteConfig subject_;

public:
    TestSuiteConfigBuilder& setSlug(string slug) {
        subject_.slug_ = slug;
        return *this;
    }

    TestSuiteConfigBuilder& setTestCasesDir(string testCasesDir) {
        subject_.testCasesDir_ = testCasesDir;
        return *this;
    }

    TestSuiteConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    TestSuiteConfig build() {
        return move(subject_);
    }
};

}}
