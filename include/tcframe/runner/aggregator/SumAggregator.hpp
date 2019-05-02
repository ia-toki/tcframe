#pragma once

#include <algorithm>
#include <vector>

#include "TestCaseAggregator.hpp"
#include "tcframe/runner/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

class SumAggregator : public TestCaseAggregator {
public:
    virtual ~SumAggregator() = default;

    SubtaskVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double subtaskPoints) {
        if (testCaseVerdicts.empty()) {
            return {Verdict::ac(), subtaskPoints};
        }

        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = 0;
        for (const TestCaseVerdict& testCaseVerdict : testCaseVerdicts) {
            aggregatedVerdict = max(aggregatedVerdict, testCaseVerdict.verdict());
            if (testCaseVerdict.verdict() == Verdict::ac()) {
                aggregatedPoints += subtaskPoints / testCaseVerdicts.size();
            } else if (testCaseVerdict.verdict() == Verdict::ok()) {
                aggregatedPoints += testCaseVerdict.points().value();
            }
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
