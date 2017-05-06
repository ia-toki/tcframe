#pragma once

#include <algorithm>
#include <vector>

#include "Aggregator.hpp"
#include "tcframe/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

class SumAggregator : public Aggregator {
public:
    virtual ~SumAggregator() {}

    Verdict aggregate(const vector<Verdict>& verdicts) {
        Verdict aggregatedVerdict;
        for (const Verdict& verdict : verdicts) {
            aggregatedVerdict = max(aggregatedVerdict, verdict);
        }
        return aggregatedVerdict;
    }
};

}
