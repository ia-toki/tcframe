#pragma once

#include "tcframe/generator/TestCaseException.hpp"

namespace tcframe {

class IOFormatException : public TestCaseException {
public:
    IOFormatException(string message)
            : TestCaseException(message) { }
};

}
