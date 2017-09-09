#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/grader/Grader.hpp"

namespace tcframe {

class MockGrader : public Grader {
public:
    MockGrader()
            : Grader(nullptr, nullptr, nullptr, nullptr) {}

    MOCK_METHOD2(grade, void(const ConstraintSuite&, const GradingOptions&));
};

class MockGraderFactory : public GraderFactory {
public:
    MOCK_METHOD4(create, Grader*(SpecClient*, TestCaseGrader*, Aggregator*, GraderLogger*));
};

}
