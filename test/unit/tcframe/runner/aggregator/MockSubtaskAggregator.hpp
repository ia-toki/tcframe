#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/SubtaskAggregator.hpp"

namespace tcframe {

class MockSubtaskAggregator : public SubtaskAggregator {
public:
    MOCK_METHOD1(aggregate, SubtaskVerdict(const vector<SubtaskVerdict>&));
};

}
