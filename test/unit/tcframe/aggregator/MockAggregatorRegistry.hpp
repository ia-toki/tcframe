#pragma once

#include "gmock/gmock.h"

#include "tcframe/aggregator/AggregatorRegistry.hpp"

namespace tcframe {

class MockAggregatorRegistry : public AggregatorRegistry {
public:
    MOCK_METHOD1(get, Aggregator*(bool));
};

}
