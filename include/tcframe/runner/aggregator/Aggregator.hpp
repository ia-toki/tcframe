#pragma once

#include <vector>

#include "tcframe/runner/verdict.hpp"

using std::vector;

namespace tcframe {

class Aggregator {
public:
    virtual ~Aggregator() = default;

    virtual Verdict aggregate(const vector<Verdict>& verdicts, double points) = 0;
};

}
