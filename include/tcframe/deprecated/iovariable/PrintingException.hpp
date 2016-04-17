#pragma once

#include "tcframe/deprecated/generator/TestCaseException.hpp"

#include <string>

using std::string;

namespace tcframe_old {

class PrintingException : public TestCaseException {
public:
    PrintingException(string message)
            : TestCaseException(message) { }
};

}
