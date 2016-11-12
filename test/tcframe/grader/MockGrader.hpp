#pragma once

#include "gmock/gmock.h"

#include "tcframe/grader/Grader.hpp"

namespace tcframe {

class MockGrader : public Grader {
public:
    MockGrader()
            : Grader(nullptr, nullptr) {}

    MOCK_METHOD3(grade, void(const TestSuite&, const set<int>&, const GraderConfig&));
};

class MockGraderFactory : public GraderFactory {
public:
    MOCK_METHOD2(create, Grader*(TestCaseGrader*, GraderLogger*));
};

}
