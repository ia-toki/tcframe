#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ScoringResult {
    friend class ScoringResultBuilder;

private:
    optional<ExecutionResult> executionResult_;
    Verdict verdict_;
    optional<string> privateInfo_;

public:
    const optional<ExecutionResult>& executionResult() const {
        return executionResult_;
    }

    const Verdict& verdict() const {
        return verdict_;
    }

    const optional<string>& privateInfo() const {
        return privateInfo_;
    }

    bool operator==(const ScoringResult& o) const {
        return tie(executionResult_, verdict_, privateInfo_)
               == tie(o.executionResult_, o.verdict_, o.privateInfo_);
    }
};

class ScoringResultBuilder {
private:
    ScoringResult subject_;

public:
    ScoringResultBuilder& setExecutionResult(ExecutionResult executionResult) {
        subject_.executionResult_ = optional<ExecutionResult>(executionResult);
        return *this;
    }

    ScoringResultBuilder& setVerdict(Verdict verdict) {
        subject_.verdict_ = verdict;
        return *this;
    }

    ScoringResultBuilder& setPrivateInfo(string privateInfo) {
        subject_.privateInfo_ = privateInfo;
        return *this;
    }

    ScoringResult build() {
        return move(subject_);
    }
};

}
