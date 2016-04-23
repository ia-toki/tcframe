#pragma once

#include "tcframe/generation/TestCaseGenerationFailure.hpp"

namespace tcframe {

class FakeTestCaseGenerationFailure : public TestCaseGenerationFailure {
public:
    FailureType type() const {
        return FailureType::VERIFICATION;
    }

protected:
    bool equals(const TestCaseGenerationFailure& o) const {
        return this == &o;
    }
};

}
