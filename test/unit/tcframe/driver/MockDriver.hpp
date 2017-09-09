#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/Driver.hpp"

namespace tcframe {

template<typename TProblemSpec>
class MockDriver : public Driver<TProblemSpec> {
public:
    MockDriver()
            : Driver<TProblemSpec>("", nullptr) {}

    MOCK_METHOD0_T(getSpecDriver, SpecDriver*());
    MOCK_METHOD0_T(buildSpec, Spec());
};

}
