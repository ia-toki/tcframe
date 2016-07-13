#pragma once

#include <vector>

#include "TestCaseGenerationResult.hpp"
#include "tcframe/generator/failure.hpp"

using std::vector;

namespace tcframe {

struct TestGroupGenerationResult {
private:
    Failure* testGroupFailure_;
    vector<TestCaseGenerationResult> testCaseResults_;

public:
    TestGroupGenerationResult(
            Failure* testGroupFailure,
            const vector<TestCaseGenerationResult>& testCaseResults)
            : testGroupFailure_(testGroupFailure)
            , testCaseResults_(testCaseResults)
    {}

    Failure* testGroupFailure() const {
        return testGroupFailure_;
    }

    const vector<TestCaseGenerationResult>& testCaseResults() const {
        return testCaseResults_;
    }

    bool isSuccessful() const {
        if (testGroupFailure_ != nullptr) {
            return false;
        }
        for (auto testCaseResult : testCaseResults_) {
            if (!testCaseResult.isSuccessful()) {
                return false;
            }
        }
        return true;
    }

    bool operator==(const TestGroupGenerationResult& o) const {
        return failuresEqual(testGroupFailure_, o.testGroupFailure_) && testCaseResults_ == o.testCaseResults_;
    }

private:
    static bool failuresEqual(Failure* failure1, Failure* failure2) {
        if (failure1 != nullptr && failure2 != nullptr) {
            return failure1->equals(failure2);
        }
        return failure1 == failure2;
    }
};

}
