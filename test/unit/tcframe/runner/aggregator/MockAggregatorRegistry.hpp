#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/AggregatorRegistry.hpp"

namespace tcframe {

class MockAggregatorRegistry : public AggregatorRegistry {
public:
    MOCK_METHOD3(getTestCaseAggregator, TestCaseAggregator*(const string&, const string&, bool));
    MOCK_METHOD0(getSubtaskAggregator, SubtaskAggregator*());
};

}
