#pragma once

#include "gmock/gmock.h"

#include "tcframe/type/Scalar.hpp"

namespace tcframe {

class MockScalar : public Scalar {
public:
    MOCK_CONST_METHOD0(getName, const string&());
    MOCK_METHOD1(parseFrom, void(istream*));
    MOCK_METHOD1(printTo, void(ostream*));
};

}
