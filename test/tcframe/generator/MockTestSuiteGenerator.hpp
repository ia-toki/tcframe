#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/TestSuiteGenerator.hpp"

namespace tcframe {

class MockTestSuiteGenerator : public TestSuiteGenerator {
public:
    MockTestSuiteGenerator()
            : TestSuiteGenerator(nullptr, nullptr, nullptr, nullptr)
    {}

    MOCK_METHOD2(generate, GenerationResult(const TestSuite&, const GeneratorConfig&));
};

class MockTestSuiteGeneratorFactory : public TestSuiteGeneratorFactory {
public:
    MOCK_METHOD4(create, TestSuiteGenerator*(TestGroupGenerator*, IOManipulator*, OperatingSystem*, GeneratorLogger*));
};

}
