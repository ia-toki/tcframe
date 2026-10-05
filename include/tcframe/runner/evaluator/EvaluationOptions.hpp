#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "SolutionMap.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct EvaluationOptions {
    friend class EvaluationOptionsBuilder;

private:
    SolutionMap solutions_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const SolutionMap& solutions() const {
        return solutions_;
    }

    string solutionCommand() const {
        return solutions_.defaultCommand();
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const EvaluationOptions& o) const {
        return tie(solutions_, timeLimit_, memoryLimit_) ==
               tie(o.solutions_, o.timeLimit_, o.memoryLimit_);
    }
};

class EvaluationOptionsBuilder {
private:
    EvaluationOptions subject_;

public:
    explicit EvaluationOptionsBuilder(EvaluationOptions from)
            : subject_(move(from)) {}

    EvaluationOptionsBuilder() = default;

    EvaluationOptionsBuilder& setSolutions(SolutionMap solutions) {
        subject_.solutions_ = move(solutions);
        return *this;
    }

    EvaluationOptionsBuilder& setSolutionCommand(const string& solutionCommand) {
        subject_.solutions_.set(SolutionMap::DEFAULT_KEY, solutionCommand);
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
