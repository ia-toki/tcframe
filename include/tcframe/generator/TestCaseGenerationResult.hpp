#pragma once

#include <string>

#include "tcframe/generator/failure.hpp"

using std::string;

namespace tcframe {

struct TestCaseGenerationResult {
private:
    Failure* failure_;

private:
    TestCaseGenerationResult(Failure* failure)
            : failure_(failure)
    {}

public:
    static TestCaseGenerationResult successfulResult() {
        return TestCaseGenerationResult(nullptr);
    }

    static TestCaseGenerationResult failedResult(Failure* failure) {
        return TestCaseGenerationResult(failure);
    }

    bool isSuccessful() const {
        return failure_ == nullptr;
    }

    Failure* failure() const {
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
