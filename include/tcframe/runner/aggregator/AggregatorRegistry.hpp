#pragma once

#include "MinAggregator.hpp"
#include "MinAggregator.hpp"
#include "SubtaskAggregator.hpp"
#include "SumAggregator.hpp"
#include "TestCaseAggregator.hpp"

namespace tcframe {

class AggregatorRegistry {
public:
    virtual ~AggregatorRegistry() = default;

    virtual TestCaseAggregator* getTestCaseAggregator(bool hasSubtasks) {
        if (hasSubtasks) {
            return new MinAggregator();
        }
        return new SumAggregator();
    }

    virtual SubtaskAggregator* getSubtaskAggregator() {
        return new SubtaskAggregator();
    }
};

}
