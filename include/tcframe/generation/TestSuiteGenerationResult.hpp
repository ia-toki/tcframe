#pragma once

#include <map>
#include <string>
#include <utility>

#include "TestCaseGenerationResult.hpp"

using std::map;
using std::move;
using std::string;

namespace tcframe {

struct TestSuiteGenerationResult {
private:
    map<string, TestCaseGenerationResult> testCaseGenerationResultByTestCaseName_;

public:
    TestSuiteGenerationResult(const map<string, TestCaseGenerationResult>& testCaseGenerationResultByTestCaseName)
            : testCaseGenerationResultByTestCaseName_(testCaseGenerationResultByTestCaseName)
    {}

    const map<string, TestCaseGenerationResult>& testCaseGenerationResultByTestCaseName() const {
        return testCaseGenerationResultByTestCaseName_;
    }

    bool isSuccessful() const {
        for (auto entry : testCaseGenerationResultByTestCaseName_) {
            if (!entry.second.isSuccessful()) {
                return false;
            }
        }
        return true;
    }
};

}
