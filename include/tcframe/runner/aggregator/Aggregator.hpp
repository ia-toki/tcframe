#pragma once

#include <vector>

#include "tcframe/runner/verdict.hpp"

using std::vector;

namespace tcframe {

class Aggregator {
public:
    virtual ~Aggregator() = default;

    virtual TestCaseVerdict aggregate(const vector<TestCaseVerdict>& testCaseVerdicts, double points) = 0;
};

}
