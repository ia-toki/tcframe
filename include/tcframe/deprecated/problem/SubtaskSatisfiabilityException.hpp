#pragma once

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/generator/TestCaseException.hpp"

#include <vector>

using std::vector;

namespace tcframe_old {

class SubtaskSatisfiabilityException : public TestCaseException {
public:
    SubtaskSatisfiabilityException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}
