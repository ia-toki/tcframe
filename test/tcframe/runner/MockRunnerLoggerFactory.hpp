#pragma once

#include "tcframe/runner/RunnerLoggerFactory.hpp"

namespace tcframe {

class MockRunnerLoggerFactory : public RunnerLoggerFactory {
public:
    MOCK_METHOD1(create, RunnerLogger*(LoggerEngine*));
};

}
