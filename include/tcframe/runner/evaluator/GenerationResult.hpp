#pragma once

#include <tuple>
#include <utility>

#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct GenerationResult {
private:
    optional<TestCaseVerdict> verdict_;
    ExecutionResult executionResult_;

public:
    GenerationResult(optional<TestCaseVerdict> verdict, ExecutionResult executionResult)
            : verdict_(move(verdict))
            , executionResult_(move(executionResult)) {}

    const optional<TestCaseVerdict>& verdict() const {
        return verdict_;
    }

    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    bool operator==(const GenerationResult& o) const {
        return tie(verdict_, executionResult_) == tie(o.verdict_, o.executionResult_);
    }
};

}
