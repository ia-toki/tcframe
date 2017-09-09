#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/SpecDriver.hpp"

namespace tcframe {

class MockSpecDriver : public SpecDriver {
public:
    MockSpecDriver()
            : SpecDriver(nullptr, TestSuite()) {}

    MOCK_METHOD0(getTestSuite, TestSuite());
    MOCK_METHOD2(generateTestCaseInput, void(const string&, ostream*));
    MOCK_METHOD2(generateSampleTestCaseOutput, void(const string&, ostream*));
    MOCK_METHOD1(validateTestCaseOutput, void(istream*));
    MOCK_METHOD1(validateMultipleTestCasesInput, void(int));
};

}
