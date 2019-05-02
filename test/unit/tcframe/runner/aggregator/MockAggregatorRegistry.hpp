#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/AggregatorRegistry.hpp"

namespace tcframe {

class MockAggregatorRegistry : public AggregatorRegistry {
public:
    MOCK_METHOD1(getTestCaseAggregator, TestCaseAggregator*(bool));
    MOCK_METHOD0(getSubtaskAggregator, SubtaskAggregator*());
};

}
