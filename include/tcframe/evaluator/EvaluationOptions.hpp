#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct EvaluationOptions {
    friend class EvaluationOptionsBuilder;

private:
    string solutionCommand_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const EvaluationOptions& o) const {
        return tie(solutionCommand_, timeLimit_, memoryLimit_) ==
               tie(o.solutionCommand_, o.timeLimit_, o.memoryLimit_);
    }
};

class EvaluationOptionsBuilder {
private:
    EvaluationOptions subject_;

public:
    EvaluationOptionsBuilder(const EvaluationOptions& from)
            : subject_(from) {}

    EvaluationOptionsBuilder() {}

    EvaluationOptionsBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    EvaluationOptionsBuilder& setTimeLimit(optional<int> timeLimit) {
        if (timeLimit) {
            setTimeLimit(timeLimit.value());
        }
        return *this;
    }

    EvaluationOptionsBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    EvaluationOptionsBuilder& setMemoryLimit(optional<int> memoryLimit) {
        if (memoryLimit) {
            setMemoryLimit(memoryLimit.value());
        }
        return *this;
    }

    EvaluationOptionsBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    EvaluationOptions build() {
        return move(subject_);
    }
};

}
