#pragma once

#include "tcframe/core/Failure.hpp"
#include "tcframe/generator/TestCaseException.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class ExecutionException : public TestCaseException {
public:
    ExecutionException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}