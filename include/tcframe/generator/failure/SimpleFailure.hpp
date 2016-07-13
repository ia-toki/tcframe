#pragma once

#include <string>
#include <tuple>

#include "Failure.hpp"

using std::string;
using std::tie;

namespace tcframe {

class SimpleFailure : public Failure {
private:
    string message_;

public:
    SimpleFailure(const string& message)
            : message_(message)
    {}

    FailureType type() const {
        return FailureType::SIMPLE;
    }

    const string& message() const {
        return message_;
    }

    bool operator==(const SimpleFailure& o) const {
        return tie(message_) == tie(o.message_);
    }

    bool equals(Failure* o) const {
        return type() == o->type() && *this == *((SimpleFailure*) o);
    }
};

}
