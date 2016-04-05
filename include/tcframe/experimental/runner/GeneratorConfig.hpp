#pragma once

#include <algorithm>
#include <string>

using std::move;
using std::string;

namespace tcframe { namespace experimental {

struct GeneratorConfig {
    friend class GeneratorConfigBuilder;

private:
    string testCasesDir_;

public:
    const string& testCasesDir() {
        return testCasesDir_;
    }
};

class GeneratorConfigBuilder {
private:
    GeneratorConfig subject_;

public:
    GeneratorConfigBuilder() {
        subject_.testCasesDir_ = "tc";
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
