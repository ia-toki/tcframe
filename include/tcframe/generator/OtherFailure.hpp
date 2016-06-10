#pragma once

#include <string>
#include <tuple>

#include "TestCaseGenerationFailure.hpp"

using std::string;
using std::tie;

namespace tcframe {

class OtherFailure : public TestCaseGenerationFailure {
private:
    string message_;

public:
    OtherFailure(const string& message)
            : message_(message)
    {}

    FailureType type() const {
        return FailureType::OTHER;
    }

    const string& message() const {
        return message_;
    }

    bool operator==(const OtherFailure& o) const {
        return tie(message_) == tie(o.message_);
    }

    bool equals(TestCaseGenerationFailure* o) const {
        return type() == o->type() && *this == *((OtherFailure*) o);
    }
};

}
