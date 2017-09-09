#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/generator/Generator.hpp"

namespace tcframe {

class MockGenerator : public Generator {
public:
    MockGenerator()
            : Generator(nullptr, nullptr, nullptr, nullptr, nullptr) {}

    MOCK_METHOD1(generate, bool(const GenerationOptions&));
};

class MockGeneratorFactory : public GeneratorFactory {
public:
    MOCK_METHOD5(create, Generator*(SeedSetter*, SpecClient*, TestCaseGenerator*, OperatingSystem*, GeneratorLogger*));
};

}
