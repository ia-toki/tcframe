#pragma once

#include <tuple>

#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::tie;

namespace tcframe {

struct GenerationResult {
private:
    optional<Verdict> verdict_;
    ExecutionResult executionResult_;

public:
    GenerationResult(const optional<Verdict>& verdict, const ExecutionResult& executionResult)
            : verdict_(verdict)
            , executionResult_(executionResult) {}

    const optional<Verdict>& verdict() const {
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
