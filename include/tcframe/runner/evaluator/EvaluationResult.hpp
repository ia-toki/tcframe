#pragma once

#include <map>
#include <string>
#include <tuple>

#include "tcframe/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::string;
using std::tie;

namespace tcframe {

struct EvaluationResult {
private:
    Verdict verdict_;
    map<string, ExecutionResult> executionResults_;

public:
    EvaluationResult(const Verdict& verdict, const map<string, ExecutionResult>& executionResults)
            : verdict_(verdict)
            , executionResults_(executionResults) {}

    const Verdict& verdict() const {
        return verdict_;
    }

    const map<string, ExecutionResult>& executionResults() const {
        return executionResults_;
    }

    bool operator==(const EvaluationResult& o) const {
        return tie(verdict_, executionResults_) == tie(o.verdict_, o.executionResults_);
    }
};

}
