#pragma once

#include <exception>

#include "tcframe/generation/TestCaseGenerationFailure.hpp"

using std::exception;

namespace tcframe { namespace experimental {

struct TestCaseGenerationException : public exception {
private:
    TestCaseGenerationFailure* failure_;

public:
    TestCaseGenerationException(TestCaseGenerationFailure* failure)
            : failure_(failure)
    {}

    TestCaseGenerationFailure* failure() const {
        return failure_;
    }
};

}}
