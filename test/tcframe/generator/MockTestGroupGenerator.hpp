#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/TestGroupGenerator.hpp"

namespace tcframe {

class MockTestGroupGenerator : public TestGroupGenerator {
public:
    MockTestGroupGenerator()
            : TestGroupGenerator(nullptr, nullptr, nullptr, nullptr)
    {}

    MOCK_METHOD2(generate, TestGroupGenerationResult(const TestGroup&, const CoreConfig&));
};

}
