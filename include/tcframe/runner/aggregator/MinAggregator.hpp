#pragma once

#include <algorithm>
#include <vector>

#include "Aggregator.hpp"
#include "tcframe/runner/verdict.hpp"

using std::max;
using std::min;
using std::vector;

namespace tcframe {

class MinAggregator : public Aggregator {
public:
    virtual ~MinAggregator() = default;

    TestCaseVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double points) {
        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = points;
        for (const TestCaseVerdict& testCaseVerdict : testCaseVerdicts) {
            aggregatedVerdict = max(aggregatedVerdict, testCaseVerdict.verdict());

            if (testCaseVerdict.verdict() == Verdict::ok()) {
                aggregatedPoints = min(aggregatedPoints, testCaseVerdict.points().value());
            } else if (!(testCaseVerdict.verdict() == Verdict::ac())) {
                aggregatedPoints = 0;
            }
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
