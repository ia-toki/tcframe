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
                double points = 0.0;
                if (testCaseVerdict.percentage()) {
                    points = testCaseVerdict.percentage().value() * subtaskPoints / 100.0;
                } else if (testCaseVerdict.points()) {
                    points = testCaseVerdict.points().value();
                }
                aggregatedPoints = min(aggregatedPoints, points);
            } else if (!(testCaseVerdict.verdict() == Verdict::ac())) {
                aggregatedPoints = 0;
            }
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
