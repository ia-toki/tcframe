#pragma once

#include <string>

#include "Aggregator.hpp"
#include "MinAggregator.hpp"
#include "SumAggregator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::string;

namespace tcframe {

class AggregatorFactory {
public:
    virtual ~AggregatorFactory() {}

    virtual Aggregator* create(bool hasSubtasks) {
        if (hasSubtasks) {
            return new MinAggregator();
        }
        return new SumAggregator();
    }
};

}
