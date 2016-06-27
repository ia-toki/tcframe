#pragma once

#include <string>

#include "tcframe/failure/Failure.hpp"

using std::string;

namespace tcframe {

struct MultipleTestCasesCombinationResult {
private:
    Failure* failure_;

public:
    MultipleTestCasesCombinationResult(Failure* failure)
            : failure_(failure)
    {}

    bool isSuccessful() const {
        return failure_ == nullptr;
    }

    Failure* failure() const {
        return failure_;
    }

    bool operator==(const MultipleTestCasesCombinationResult& o) const {
        if (failure_ != nullptr && o.failure_ != nullptr) {
            return failure_->equals(o.failure_);
        }
        return failure_ == o.failure_;
    }
};

}
