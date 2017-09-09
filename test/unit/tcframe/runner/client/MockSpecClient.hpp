#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/client/SpecClient.hpp"

namespace tcframe {

class MockSpecClient : public SpecClient {
public:
    MockSpecClient()
            : SpecClient(nullptr, nullptr) {}

    MOCK_METHOD0(getTestSuite, TestSuite());
    MOCK_METHOD2(generateTestCaseInput, void(const string&, const string&));
    MOCK_METHOD2(generateSampleTestCaseOutput, void(const string&, const string&));
    MOCK_METHOD1(validateTestCaseOutput, void(const string&));
    MOCK_METHOD1(validateMultipleTestCasesInput, void(int));
};

}
