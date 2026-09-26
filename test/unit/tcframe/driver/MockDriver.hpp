#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/Driver.hpp"

namespace tcframe {

template<typename TProblemSpec>
class MockDriver : public Driver<TProblemSpec> {
public:
    MockDriver()
            : Driver<TProblemSpec>("", nullptr) {}

    MOCK_METHOD1_T(buildSpec, pair<SpecYaml, SpecDriver*>(const Args& args));
};

}
