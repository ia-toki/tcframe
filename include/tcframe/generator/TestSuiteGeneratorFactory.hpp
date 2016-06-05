#pragma once

#include "TestSuiteGenerator.hpp"

namespace tcframe {

class TestSuiteGeneratorFactory {
public:
    virtual ~TestSuiteGeneratorFactory() {}

    virtual TestSuiteGenerator* create(
            TestCaseGenerator* testCaseGenerator,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GeneratorLogger* logger) {
        return new TestSuiteGenerator(testCaseGenerator, ioManipulator, os, logger);
    }
};

}
