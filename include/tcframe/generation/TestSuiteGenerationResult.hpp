#pragma once

#include <map>
#include <string>
#include <utility>

using std::map;
using std::move;
using std::string;

namespace tcframe {

struct TestSuiteGenerationResult {
private:
    map<string, TestCaseGenerationResult> resultsByName_;

public:
    TestSuiteGenerationResult(const map<string, TestCaseGenerationResult>& resultsByName)
            : resultsByName_(resultsByName)
    {}

    const map<string, TestCaseGenerationResult>& resultsByName() const {
        return resultsByName_;
    }

    bool isSuccessful() const {
        for (auto entry : resultsByName_) {
            if (!entry.second.isSuccessful()) {
                return false;
            }
        }
        return true;
    }
};

}
