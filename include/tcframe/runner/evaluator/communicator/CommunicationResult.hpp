#pragma once

#include <tuple>
#include <utility>

#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct CommunicationResult {
private:
    Verdict verdict_;
    ExecutionResult executionResult_;

public:
    CommunicationResult(Verdict verdict, ExecutionResult executionResult)
            : verdict_(move(verdict))
            , executionResult_(move(executionResult)) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    bool operator==(const CommunicationResult& o) const {
        return tie(verdict_, executionResult_) == tie(o.verdict_, o.executionResult_);
    }
};

}
