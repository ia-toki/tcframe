#pragma once

#include "gmock/gmock.h"

#include "tcframe/generator/GeneratorLogger.hpp"

namespace tcframe {

class MockGeneratorLogger : public GeneratorLogger {
public:
    MockGeneratorLogger()
            : GeneratorLogger(nullptr)
    {}

    MOCK_METHOD0(logIntroduction, void());
    MOCK_METHOD1(logResult, void(const GenerationResult&));
    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(string));
    MOCK_METHOD2(logTestCaseResult, void(const string&, const TestCaseGenerationResult&));
    MOCK_METHOD1(logMultipleTestCasesCombinationIntroduction, void(const string&));
    MOCK_METHOD1(logMultipleTestCasesCombinationResult, void(const MultipleTestCasesCombinationResult&));
};

}
