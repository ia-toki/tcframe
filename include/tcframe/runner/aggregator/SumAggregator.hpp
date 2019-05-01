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

    TestCaseVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double points) {
        if (testCaseVerdicts.empty()) {
            return {Verdict::ac(), points};
        }

        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = 0;
        for (const TestCaseVerdict& testCaseVerdict : testCaseVerdicts) {
            aggregatedVerdict = max(aggregatedVerdict, testCaseVerdict.verdict());
            if (testCaseVerdict.verdict() == Verdict::ac()) {
                aggregatedPoints += points / testCaseVerdicts.size();
            } else if (testCaseVerdict.verdict() == Verdict::ok()) {
                aggregatedPoints += testCaseVerdict.points().value();
            }
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
