#pragma once

#include "gmock/gmock.h"

#include "tcframe/spec/core/SeedSetter.hpp"

namespace tcframe {

class MockSeedSetter : public SeedSetter {
public:
    MockSeedSetter()
            : SeedSetter([](unsigned){}) {}

    MOCK_METHOD1(setSeed, void(unsigned));
};

}
