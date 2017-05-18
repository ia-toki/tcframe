#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/TestCaseGenerator.hpp"

namespace tcframe {

class MockTestCaseGenerator : public TestCaseGenerator {
public:
    MockTestCaseGenerator()
            : TestCaseGenerator(nullptr, nullptr, nullptr, nullptr, nullptr) {}

    MOCK_METHOD2(generate, bool(const TestCase&, const GenerationOptions&));
};

}
