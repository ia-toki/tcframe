#pragma once

#include "gmock/gmock.h"

#include "tcframe/submitter/TestCaseSubmitter.hpp"

namespace tcframe {

class MockTestCaseSubmitter : public TestCaseSubmitter {
public:
    MockTestCaseSubmitter()
            : TestCaseSubmitter(nullptr, nullptr, nullptr) {}

    MOCK_METHOD2(submit, Verdict(const TestCase&, const SubmitterConfig&));
};

}
