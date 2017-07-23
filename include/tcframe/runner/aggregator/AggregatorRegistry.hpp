#pragma once

#include "Aggregator.hpp"
#include "MinAggregator.hpp"
#include "SumAggregator.hpp"

namespace tcframe {

class AggregatorRegistry {
public:
    virtual ~AggregatorRegistry() {}

    virtual Aggregator* get(bool hasSubtasks) {
        if (hasSubtasks) {
            return new MinAggregator();
        }
        return new SumAggregator();
    }
};

}
