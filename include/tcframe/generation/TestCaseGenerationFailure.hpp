#pragma once

namespace tcframe {

enum FailureType {
    VERIFICATION
};

struct TestCaseGenerationFailure {
public:
    virtual ~TestCaseGenerationFailure() {}

    virtual FailureType type() const = 0;

    bool operator==(const TestCaseGenerationFailure& o) const {
        return type() == o.type() && equals(o);
    }

protected:
    virtual bool equals(const TestCaseGenerationFailure& o) const = 0;
};

}
