#pragma once

#include "gmock/gmock.h"

#include <istream>
#include <ostream>

#include "tcframe/type/Scalar.hpp"

using std::istream;
using std::ostream;

namespace tcframe {

class MockScalar : public Scalar {
public:
    MOCK_CONST_METHOD0(getName, const string&());
    MOCK_METHOD1(parseFrom, void(istream*));
    MOCK_METHOD1(printTo, void(ostream*));
};

}
