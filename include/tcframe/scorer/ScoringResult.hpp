#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/os.hpp"
#include "tcframe/verdict.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ScoringResult {
    friend class ScoringResultBuilder;

private:
    ExecutionResult executionResult_;
    Verdict verdict_;
    string message_;

public:
    const ExecutionResult& executionResult() const {
        return executionResult_;
    }

    const Verdict& verdict() const {
        return verdict_;
    }

    const string& message() const {
        return message_;
    }

    bool operator==(const ScoringResult& o) const {
        return tie(executionResult_, verdict_, message_) == tie(o.executionResult_, o.verdict_, o.message_);
    }
};

class ScoringResultBuilder {
private:
    ScoringResult subject_;

public:
    ScoringResultBuilder& setExecutionResult(ExecutionResult executionResult) {
        subject_.executionResult_ = executionResult;
        return *this;
    }

    ScoringResultBuilder& setVerdict(Verdict verdict) {
        subject_.verdict_ = verdict;
        return *this;
    }

    ScoringResultBuilder& setMessage(string message) {
        subject_.message_ = message;
        return *this;
    }

    ScoringResult build() {
        return move(subject_);
    }
};

}
