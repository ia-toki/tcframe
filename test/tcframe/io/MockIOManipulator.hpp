#pragma once

#include "gmock/gmock.h"

#include "tcframe/io/IOManipulator.hpp"

namespace tcframe {

class MockIOManipulator : public IOManipulator {
public:
    MockIOManipulator()
            : IOManipulator(nullptr, IOFormat())
    {}

    MOCK_METHOD1(printInput, void(ostream*));
};

}
