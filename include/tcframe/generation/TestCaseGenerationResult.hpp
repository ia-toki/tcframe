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
    TestCaseGenerationResult() = default;

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

    bool operator==(const TestCaseGenerationResult& o) const {
        if (failure_ != nullptr && o.failure_ != nullptr) {
            return failure_->equals(o.failure_);
        }
        return failure_ == o.failure_;
    }
};

}
