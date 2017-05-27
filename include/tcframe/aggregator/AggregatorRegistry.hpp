#pragma once

#include "tcframe/aggregator/Aggregator.hpp"
#include "tcframe/aggregator/MinAggregator.hpp"
#include "tcframe/aggregator/SumAggregator.hpp"

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
