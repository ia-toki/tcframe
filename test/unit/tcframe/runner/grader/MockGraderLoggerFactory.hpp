#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/grader/GraderLoggerFactory.hpp"

namespace tcframe {

class MockGraderLoggerFactory : public GraderLoggerFactory {
public:
    MOCK_METHOD2(create, GraderLogger*(LoggerEngine*, bool));
};

}
