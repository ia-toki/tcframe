#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/grader/GraderLoggerFactory.hpp"

namespace tcframe {

class MockGraderLoggerFactory : public GraderLoggerFactory {
public:
    MOCK_METHOD3(create, GraderLogger*(LoggerEngine*, bool, bool));
};

}
