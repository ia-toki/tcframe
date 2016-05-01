#pragma once

#include "gmock/gmock.h"

#include "tcframe/variable/Scalar.hpp"

namespace tcframe {

class MockScalar : public Scalar {
public:
    MockScalar()
            : Scalar("mock")
    {}

    MOCK_METHOD1(parseFrom, void(istream*));
    MOCK_METHOD1(printTo, void(ostream*));
};

}
