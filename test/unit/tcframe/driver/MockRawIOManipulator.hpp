#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/RawIOManipulator.hpp"

namespace tcframe {

class MockRawIOManipulator : public RawIOManipulator {
public:
    MOCK_METHOD2(print, void(ostream*, const string&));
    MOCK_METHOD2(printLine, void(ostream*, const string&));
};

}
