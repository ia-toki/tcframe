#pragma once

#include "gmock/gmock.h"

#include "tcframe/aggregator/Aggregator.hpp"
#include "tcframe/aggregator/AggregatorFactory.hpp"

namespace tcframe {

class MockAggregator : public Aggregator {
public:
    MOCK_METHOD2(aggregate, Verdict(const vector<Verdict>&, double));
};

class MockAggregatorFactory : public AggregatorFactory {
public:
    MOCK_METHOD1(create, Aggregator*(bool));
};

}
