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

struct EvaluatorConfig {
    friend class EvaluatorConfigBuilder;

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

    bool operator==(const EvaluatorConfig& o) const {
        return tie(solutionCommand_, timeLimit_, memoryLimit_) ==
                tie(o.solutionCommand_, o.timeLimit_, o.memoryLimit_);
    }
};

class EvaluatorConfigBuilder {
private:
    EvaluatorConfig subject_;

public:
    EvaluatorConfigBuilder(const EvaluatorConfig& from)
            : subject_(from) {}

    EvaluatorConfigBuilder() {
        subject_.solutionCommand_ = CommonConfig::solutionCommand();
    }

    EvaluatorConfigBuilder& setSolutionCommand(optional<string> solutionCommand) {
        if (solutionCommand) {
            setSolutionCommand(solutionCommand.value());
        }
        return *this;
    }

    EvaluatorConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    EvaluatorConfigBuilder& setTimeLimit(optional<int> timeLimit) {
        if (timeLimit) {
            setTimeLimit(timeLimit.value());
        }
        return *this;
    }

    EvaluatorConfigBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    EvaluatorConfigBuilder& setMemoryLimit(optional<int> memoryLimit) {
        if (memoryLimit) {
            setMemoryLimit(memoryLimit.value());
        }
        return *this;
    }

    EvaluatorConfigBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    EvaluatorConfig build() {
        return move(subject_);
    }
};

}
