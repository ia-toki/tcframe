#pragma once

#include <string>
#include <tuple>

#include "Failure.hpp"
#include "tcframe/os.hpp"

using std::string;
using std::tie;

namespace tcframe {

class SolutionExecutionFailure : public Failure {
private:
    ExecutionResult executionResult_;

public:
    SolutionExecutionFailure(const ExecutionResult& executionResult)
            : executionResult_(executionResult)
    {}

    FailureType type() const {
        return FailureType::SOLUTION_EXECUTION;
    }

    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    bool operator==(const SolutionExecutionFailure& o) const {
        return tie(executionResult_) == tie(o.executionResult_);
    }

    bool equals(Failure* o) const {
        return type() == o->type() && *this == *((SolutionExecutionFailure*) o);
    }
};

}
