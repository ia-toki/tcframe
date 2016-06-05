#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/RunnerLogger.hpp"

namespace tcframe {

class MockRunnerLogger : public RunnerLogger {
public:
    MockRunnerLogger()
            : RunnerLogger(nullptr)
    {}

    MOCK_METHOD1(logSpecificationFailure, void(SpecificationFailure failure));
};

}
