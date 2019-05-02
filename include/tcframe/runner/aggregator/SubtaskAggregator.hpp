#pragma once

#include <algorithm>
#include <vector>

#include "tcframe/runner/verdict.hpp"

using std::max;
using std::vector;

namespace tcframe {

class SubtaskAggregator {
public:
    virtual ~SubtaskAggregator() = default;

    virtual SubtaskVerdict aggregate(const vector<SubtaskVerdict>& subtaskVerdicts) {
        Verdict aggregatedVerdict = Verdict::ac();
        double aggregatedPoints = 0;
        for (const SubtaskVerdict& subtaskVerdict : subtaskVerdicts) {
            aggregatedVerdict = max(aggregatedVerdict, subtaskVerdict.verdict());
            aggregatedPoints += subtaskVerdict.points();
        }
        return {aggregatedVerdict, aggregatedPoints};
    }
};

}
