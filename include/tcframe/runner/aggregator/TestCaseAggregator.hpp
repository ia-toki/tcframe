#pragma once

#include <vector>
#include "tcframe/runner/verdict.hpp"

using std::vector;

namespace tcframe {

class TestCaseAggregator {
public:
    virtual ~TestCaseAggregator() = default;

    virtual SubtaskVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double subtaskPoints) = 0;
};

}
