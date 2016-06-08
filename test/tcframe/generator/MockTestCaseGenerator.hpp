#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/TestCaseGenerator.hpp"

namespace tcframe {

class MockTestCaseGenerator : public TestCaseGenerator {
public:
    MockTestCaseGenerator()
            : TestCaseGenerator(nullptr, nullptr, nullptr)
    {}

    MOCK_METHOD3(generate, TestCaseGenerationResult(
            const TestCaseData&,
            const function<void()>&,
            const TestConfig&));
};

}
