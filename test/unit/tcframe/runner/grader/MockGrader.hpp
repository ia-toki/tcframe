#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/grader/Grader.hpp"

namespace tcframe {

class MockGrader : public Grader {
public:
    MockGrader()
            : Grader(nullptr, nullptr, nullptr) {}

    MOCK_METHOD3(grade, void(const TestSuite&, const ConstraintSuite&, const GradingOptions&));
};

class MockGraderFactory : public GraderFactory {
public:
    MOCK_METHOD3(create, Grader*(TestCaseGrader*, Aggregator*, GraderLogger*));
};

}
