#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/TestCaseAggregator.hpp"

namespace tcframe {

class MockTestCaseAggregator : public TestCaseAggregator {
public:
    MOCK_METHOD2(aggregate, SubtaskVerdict(const vector<TestCaseVerdict>&, double));
};

}
