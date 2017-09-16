#pragma once

#include <algorithm>
#include <vector>

#include "Aggregator.hpp"
#include "tcframe/runner/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

class SumAggregator : public Aggregator {
public:
    virtual ~SumAggregator() = default;

    Verdict aggregate(const vector<Verdict>& verdicts, double points) {
        if (verdicts.empty()) {
            return {VerdictStatus::ac(), points};
        }

        VerdictStatus aggregatedStatus = VerdictStatus::ac();
        double aggregatedPoints = 0;
        for (const Verdict& verdict : verdicts) {
            aggregatedStatus = max(aggregatedStatus, verdict.status());
            if (verdict.status() == VerdictStatus::ac()) {
                aggregatedPoints += points / verdicts.size();
            } else if (verdict.status() == VerdictStatus::ok()) {
                aggregatedPoints += verdict.points().value();
            }
        }
        return {aggregatedStatus, aggregatedPoints};
    }
};

}
