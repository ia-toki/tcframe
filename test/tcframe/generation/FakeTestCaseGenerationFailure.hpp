#pragma once

#include "tcframe/generation/TestCaseGenerationFailure.hpp"

namespace tcframe {

class FakeTestCaseGenerationFailure : public TestCaseGenerationFailure {
protected:
    bool equals(const TestCaseGenerationFailure& o) const {
        return this == &o;
    }
};

}
