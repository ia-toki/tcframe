#pragma once

#include <vector>

#include "GeneratorLogger.hpp"
#include "TestCase.hpp"
#include "TestCaseGenerator.hpp"
#include "TestGroup.hpp"
#include "TestGroupGenerationResult.hpp"
#include "tcframe/core.hpp"

using std::vector;

namespace tcframe {

class TestGroupGenerator {
private:
    TestCaseGenerator* testCaseGenerator_;
    GeneratorLogger* logger_;

public:
    virtual ~TestGroupGenerator() {}

    TestGroupGenerator(TestCaseGenerator* testCaseGenerator, GeneratorLogger* logger)
            : testCaseGenerator_(testCaseGenerator)
            , logger_(logger)
    {}

    virtual TestGroupGenerationResult generate(const TestGroup& testGroup, const TestConfig& testConfig) {
        logger_->logTestGroupIntroduction(testGroup.id());

        vector<TestCaseGenerationResult> testCaseResults;
        for (const TestCase& testCase : testGroup.testCases()) {
            testCaseResults.push_back(testCaseGenerator_->generate(testCase, testConfig));
        }
        return TestGroupGenerationResult(nullptr, testCaseResults);
    }
};

}
