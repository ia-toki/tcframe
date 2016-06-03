#pragma once

#include "tcframe/generator/TestCaseGenerationFailure.hpp"

namespace tcframe {

class FakeTestCaseGenerationFailure : public TestCaseGenerationFailure {
public:
    FailureType type() const {
        return FailureType::VERIFICATION;
    }

    bool operator==(const FakeTestCaseGenerationFailure& o) const {
        return type() == o.type();
    }

    bool equals(TestCaseGenerationFailure* o) const {
        return type() == o->type();
    }
};

}
