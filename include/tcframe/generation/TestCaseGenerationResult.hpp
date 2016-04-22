#pragma once

#include "TestCaseGenerationFailure.hpp"

namespace tcframe {

struct TestCaseGenerationResult {
private:
    TestCaseGenerationFailure* failure_;

private:
    TestCaseGenerationResult(TestCaseGenerationFailure* failure)
            : failure_(failure)
    {}

public:
    static TestCaseGenerationResult successfulResult() {
        return TestCaseGenerationResult(nullptr);
    }

    static TestCaseGenerationResult failedResult(TestCaseGenerationFailure* failure) {
        return TestCaseGenerationResult(failure);
    }

    bool isSuccessful() const {
        return failure_ == nullptr;
    }

    TestCaseGenerationFailure* failure() const {
        return failure_;
    }
};

}
