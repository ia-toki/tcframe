#pragma once

#include "tcframe/deprecated/generator/TestCaseException.hpp"

namespace tcframe {

class IOFormatException : public TestCaseException {
public:
    IOFormatException(string message)
            : TestCaseException(message) { }
};

}
