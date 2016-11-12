#pragma once

#include "GraderConfig.hpp"
#include "Verdict.hpp"

namespace tcframe {

class Scorer {
public:
    virtual ~Scorer() {}

    virtual Verdict score(const TestCase& testCase, const GraderConfig& config) = 0;
};

}
