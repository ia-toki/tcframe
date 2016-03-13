#pragma once

#include "tcframe/core/Failure.hpp"
#include "tcframe/generator/TestCaseException.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class SubtaskSatisfiabilityException : public TestCaseException {
public:
    SubtaskSatisfiabilityException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}
