#pragma once

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/generator/TestCaseException.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class ExecutionException : public TestCaseException {
public:
    ExecutionException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}
