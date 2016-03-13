#pragma once

#include "tcframe/core/Failure.hpp"
#include "tcframe/generator/TestCaseException.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class MultipleTestCasesConstraintsSatisfiabilityException : public TestCaseException {
public:
    MultipleTestCasesConstraintsSatisfiabilityException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}
