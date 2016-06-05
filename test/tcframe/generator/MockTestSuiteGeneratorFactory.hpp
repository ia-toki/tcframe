#pragma once

#include "tcframe/generator/TestSuiteGeneratorFactory.hpp"

namespace tcframe {

class MockTestSuiteGeneratorFactory : public TestSuiteGeneratorFactory {
public:
    MOCK_METHOD4(create, TestSuiteGenerator*(TestCaseGenerator*, IOManipulator*, OperatingSystem*, GeneratorLogger*));
};

}
