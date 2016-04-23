#pragma once

#include "gmock/gmock.h"

#include "tcframe/generation/TestSuiteGenerationListener.hpp"

namespace tcframe {

class MockTestSuiteGenerationListener : public TestSuiteGenerationListener {
public:
    MOCK_METHOD0(onIntroduction, void());
    MOCK_METHOD1(onTestGroupIntroduction, void(int));
    MOCK_METHOD1(onTestCaseIntroduction, void(string));
    MOCK_METHOD2(onTestCaseGenerationResult, void(const string&, const TestCaseGenerationResult&));
};

}
