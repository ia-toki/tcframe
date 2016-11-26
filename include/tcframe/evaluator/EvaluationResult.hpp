#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct EvaluationResult {
    friend class EvaluationResultBuilder;

private:
    ExecutionResult executionResult_;
    optional<Verdict> verdict_;

public:
    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    const optional<Verdict> verdict() const {
        return verdict_;
    }

    bool operator==(const EvaluationResult& o) const {
        return tie(executionResult_, verdict_) == tie(o.executionResult_, o.verdict_);
    }
};

class EvaluationResultBuilder {
private:
    EvaluationResult subject_;

public:
    EvaluationResultBuilder& setExecutionResult(ExecutionResult executionResult) {
        subject_.executionResult_ = executionResult;
        return *this;
    }

    EvaluationResultBuilder& setVerdict(optional<Verdict> verdict) {
        if (verdict) {
            setVerdict(verdict.value());
        }
        return *this;
    }

    EvaluationResultBuilder& setVerdict(Verdict verdict) {
        subject_.verdict_ = optional<Verdict>(verdict);
        return *this;
    }

    EvaluationResult build() {
        return move(subject_);
    }
};

}
