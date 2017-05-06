#pragma once

#include <algorithm>
#include <vector>

#include "Aggregator.hpp"
#include "tcframe/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

class MinAggregator : public Aggregator {
public:
    virtual ~MinAggregator() {}

    Verdict aggregate(const vector<Verdict>& verdicts) {
        Verdict aggregatedVerdict;
        for (const Verdict& verdict : verdicts) {
            aggregatedVerdict = max(aggregatedVerdict, verdict);
        }
        return aggregatedVerdict;
    }
};

}
