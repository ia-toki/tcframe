#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/TestCaseDriver.hpp"

namespace tcframe {

class MockTestCaseDriver : public TestCaseDriver {
public:
    MockTestCaseDriver()
            : TestCaseDriver(nullptr, nullptr, nullptr, MultipleTestCasesConfig()) {}

    MOCK_METHOD2(generateInput, void(const TestCase&, ostream*));
    MOCK_METHOD2(generateSampleOutput, void(const TestCase&, ostream*));
    MOCK_METHOD1(validateOutput, void(istream*));
    MOCK_METHOD1(validateMultipleTestCasesInput, void(int));
};

}
