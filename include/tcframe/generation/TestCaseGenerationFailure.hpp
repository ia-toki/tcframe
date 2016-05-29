#pragma once

namespace tcframe {

enum FailureType {
    VERIFICATION
};

struct TestCaseGenerationFailure {
public:
    virtual ~TestCaseGenerationFailure() {}

    virtual FailureType type() const = 0;

    virtual bool equals(TestCaseGenerationFailure* o) const = 0;
};

}
