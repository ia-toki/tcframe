#pragma once

#include <algorithm>
#include <vector>

#include "TestCaseAggregator.hpp"
#include "tcframe/runner/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

// IOI 2010 Hotter Colder style (RFC "Threshold aggregator"): the subtask scores in
// full only if no test case exceeds the threshold. A test case "OK x" passes when
// x <= threshold; "OK x" with x > threshold, or "OK" without a value, counts as WA.
// AC passes. Verdict: AC when every test case passes, else the worst failing verdict.
class ThresholdAggregator : public TestCaseAggregator {
private:
    double threshold_;

public:
    virtual ~ThresholdAggregator() = default;

    explicit ThresholdAggregator(double threshold)
            : threshold_(threshold) {}

    SubtaskVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double subtaskPoints) {
        Verdict aggregatedVerdict = Verdict::ac();
        for (const TestCaseVerdict& testCaseVerdict : testCaseVerdicts) {
            if (testCaseVerdict.verdict() == Verdict::ac()) {
                continue;
            }

            Verdict failedVerdict = testCaseVerdict.verdict();
            if (testCaseVerdict.verdict() == Verdict::ok()) {
                if (testCaseVerdict.points() && testCaseVerdict.points().value() <= threshold_) {
                    continue;
                }
                failedVerdict = Verdict::wa();
            }
            aggregatedVerdict = max(aggregatedVerdict, failedVerdict);
        }

        if (aggregatedVerdict == Verdict::ac()) {
            return {aggregatedVerdict, subtaskPoints};
        }
        return {aggregatedVerdict, 0};
    }
};

}
