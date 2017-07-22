#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/logger/RunnerLogger.hpp"

namespace tcframe {

class MockRunnerLogger : public RunnerLogger {
public:
    MockRunnerLogger()
            : RunnerLogger(nullptr) {}

    MOCK_METHOD1(logSpecificationFailure, void(const vector<string>&));
};

class MockRunnerLoggerFactory : public RunnerLoggerFactory {
public:
    MOCK_METHOD1(create, RunnerLogger*(LoggerEngine*));
};


}
