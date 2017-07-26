#pragma once

#include <tuple>

#include "tcframe/os.hpp"
#include "tcframe/runner/verdict.hpp"

using std::tie;

namespace tcframe {

struct CommunicationResult {
private:
    Verdict verdict_;
    ExecutionResult executionResult_;

public:
    CommunicationResult(const Verdict& verdict, const ExecutionResult& executionResult)
            : verdict_(verdict)
            , executionResult_(executionResult) {}

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
