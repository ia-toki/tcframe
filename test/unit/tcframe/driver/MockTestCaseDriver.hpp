#pragma once

#include "gmock/gmock.h"

#include "tcframe/driver/TestCaseDriver.hpp"
#include "tcframe/runner/core/Args.hpp"

namespace tcframe {

class MockTestCaseDriver : public TestCaseDriver {
private:
    Args args_;

public:
    MockTestCaseDriver()
            : TestCaseDriver(nullptr, nullptr, nullptr, MultipleTestCasesConfig(), args_) {}

    MOCK_METHOD2(generateInput, void(const TestCase&, ostream*));
    MOCK_METHOD2(generateSampleOutput, void(const TestCase&, ostream*));
    MOCK_METHOD1(validateOutput, void(istream*));
    MOCK_METHOD1(validateMultipleTestCasesInput, void(int));
};

}
