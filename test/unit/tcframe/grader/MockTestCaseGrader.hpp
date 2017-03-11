#pragma once

#include "gmock/gmock.h"

#include "tcframe/grader/TestCaseGrader.hpp"

namespace tcframe {

class MockTestCaseGrader : public TestCaseGrader {
public:
    MockTestCaseGrader()
            : TestCaseGrader(nullptr, nullptr, nullptr) {}

    MOCK_METHOD2(grade, TestCaseGrade(const TestCase&, const GraderConfig&));
};

}
