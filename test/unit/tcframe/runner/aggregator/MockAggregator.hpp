#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/Aggregator.hpp"

namespace tcframe {

class MockAggregator : public Aggregator {
public:
    MOCK_METHOD2(aggregate, TestCaseVerdict(const vector<TestCaseVerdict>&, double));
};

}
