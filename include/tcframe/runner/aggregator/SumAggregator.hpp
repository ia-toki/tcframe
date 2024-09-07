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

        double testCaseFullPoints = subtaskPoints / testCaseVerdicts.size();

        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = 0;
        for (const TestCaseVerdict& testCaseVerdict : testCaseVerdicts) {
            aggregatedVerdict = max(aggregatedVerdict, testCaseVerdict.verdict());

            if (testCaseVerdict.verdict() == Verdict::ac()) {
                aggregatedPoints += testCaseFullPoints;
            } else if (testCaseVerdict.verdict() == Verdict::ok()) {
                if (testCaseVerdict.points()) {
                    aggregatedPoints += testCaseVerdict.points().value();
                } else if (testCaseVerdict.percentage()) {
                    aggregatedPoints += testCaseVerdict.percentage().value() * testCaseFullPoints / 100.0;
                }
            }
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
