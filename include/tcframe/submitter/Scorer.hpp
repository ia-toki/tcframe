#pragma once

#include "SubmitterConfig.hpp"
#include "Verdict.hpp"
#include "tcframe/testcase.hpp"

namespace tcframe {

class Scorer {
public:
    virtual ~Scorer() {}

    virtual Verdict score(const TestCase& testCase, const SubmitterConfig& config) = 0;
};

}
