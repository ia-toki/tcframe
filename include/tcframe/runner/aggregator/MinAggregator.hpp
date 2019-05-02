#pragma once

#include <algorithm>
#include <vector>

#include "TestCaseAggregator.hpp"
#include "tcframe/runner/verdict.hpp"

using std::max;
using std::min;
using std::vector;

namespace tcframe {

class MinAggregator : public TestCaseAggregator {
public:
    virtual ~MinAggregator() = default;

    SubtaskVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double subtaskPoints) {
        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = subtaskPoints;
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
