#pragma once

#include "tcframe/deprecated/generator/TestCaseException.hpp"

#include <string>

using std::string;

namespace tcframe_old {
    
class ParsingException : public TestCaseException {
public:
    ParsingException(string message)
            : TestCaseException(message) { }
};

}
