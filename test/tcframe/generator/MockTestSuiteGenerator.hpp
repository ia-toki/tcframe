#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/TestSuiteGenerator.hpp"

namespace tcframe {

class MockTestSuiteGenerator : public TestSuiteGenerator {
public:
    MockTestSuiteGenerator()
            : TestSuiteGenerator(nullptr, nullptr, nullptr, nullptr)
    {}

    MOCK_METHOD3(generate, GenerationResult(const TestSuite&, const ProblemConfig&, const GeneratorConfig&));
};

}
