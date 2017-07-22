#pragma once

#include "gmock/gmock.h"

#include "tcframe/spec/io/IOManipulator.hpp"

namespace tcframe {

class MockIOManipulator : public IOManipulator {
public:
    MockIOManipulator()
            : IOManipulator(IOFormat()) {}

    MOCK_METHOD1(printInput, void(ostream*));
    MOCK_METHOD1(parseInput, void(istream*));
    MOCK_METHOD1(parseOutput, void(istream*));
};

}
