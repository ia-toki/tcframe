#pragma once

namespace tcframe {

class TestCaseGenerationFailure {
public:
    virtual ~TestCaseGenerationFailure() {}

    bool operator==(const TestCaseGenerationFailure& o) const {
        return typeid(*this) == typeid(o) && equals(o);
    }

protected:
    virtual bool equals(const TestCaseGenerationFailure& o) const = 0;
};

}
