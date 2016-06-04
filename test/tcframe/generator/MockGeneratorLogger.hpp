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
    MOCK_METHOD0(logSampleTestCasesIntroduction, void());
    MOCK_METHOD1(logTestGroupIntroduction, void(int));
    MOCK_METHOD1(logTestCaseIntroduction, void(string));
    MOCK_METHOD2(logTestCaseGenerationResult, void(const string&, const TestCaseGenerationResult&));
};

}
