#pragma once

#include "gmock/gmock.h"

#include "tcframe/grading_style/GradingStyle.hpp"

namespace tcframe {

class MockGradingStyleFactory : public GradingStyleFactory {
public:
    MOCK_METHOD2(createBatch, GradingStyle(OperatingSystem*, const optional<string>&));
};

}
