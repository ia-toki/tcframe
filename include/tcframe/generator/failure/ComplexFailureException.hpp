#pragma once

#include <exception>

#include "Failure.hpp"

using std::exception;

namespace tcframe {

struct ComplexFailureException : public exception {
private:
    Failure* failure_;

public:
    ComplexFailureException(Failure* failure)
            : failure_(failure)
    {}

    Failure* failure() const {
        return failure_;
    }
};

}
