#pragma once

#include <vector>

#include "tcframe/verdict.hpp"

using std::vector;

namespace tcframe {

class Aggregator {
public:
    virtual ~Aggregator() {}

    virtual Verdict aggregate(const vector<Verdict>& verdicts, double points) = 0;
};

}
