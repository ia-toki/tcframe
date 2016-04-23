#pragma once

#include "gmock/gmock.h"

#include "tcframe/variable/IOVariablesPrinter.hpp"

namespace tcframe {

class MockIOVariablesPrinter : public IOVariablesPrinter {
public:
    MockIOVariablesPrinter()
            : IOVariablesPrinter(nullptr, IOFormat())
    {}

    MOCK_METHOD1(printInput, void(ostream*));
};

}
