#pragma once

#include <algorithm>
#include <vector>

#include "Aggregator.hpp"
#include "tcframe/verdict.hpp"

using std::max;
using std::min;
using std::vector;

namespace tcframe {

class MinAggregator : public Aggregator {
public:
    virtual ~MinAggregator() {}

    Verdict aggregate(const vector<Verdict>& verdicts, double points) {
        VerdictStatus aggregatedStatus = VerdictStatus::ac();
        double aggregatedPoints = points;
        for (const Verdict& verdict : verdicts) {
            aggregatedStatus = max(aggregatedStatus, verdict.status());

            if (verdict.status() == VerdictStatus::ok()) {
                aggregatedPoints = min(aggregatedPoints, verdict.points().value());
            } else if (!(verdict.status() == VerdictStatus::ac())) {
                aggregatedPoints = 0;
            }
        }
        return Verdict(aggregatedStatus, aggregatedPoints);
    }
};

}
