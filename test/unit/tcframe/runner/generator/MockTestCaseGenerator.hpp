#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/generator/TestCaseGenerator.hpp"

namespace tcframe {

class MockTestCaseGenerator : public TestCaseGenerator {
public:
    MockTestCaseGenerator()
            : TestCaseGenerator(nullptr, nullptr, nullptr) {}

    MOCK_METHOD2(generate, bool(const TestCase&, const GenerationOptions&));
};

}
