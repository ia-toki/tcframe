#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/Generator.hpp"

namespace tcframe {

class MockGenerator : public Generator {
public:
    MockGenerator()
            : Generator(nullptr, nullptr, nullptr, nullptr)
    {}

    MOCK_METHOD2(generate, GenerationResult(const RawTestSuite&, const GeneratorConfig&));
};

class MockGeneratorFactory : public GeneratorFactory {
public:
    MOCK_METHOD4(create, Generator*(TestGroupGenerator*, IOManipulator*, OperatingSystem*, GeneratorLogger*));
};

}
