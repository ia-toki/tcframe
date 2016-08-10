#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ExecutionInfo {
    friend class ExecutionInfoBuilder;

private:
    optional<int> exitCode_;
    optional<string> exitSignal_;
    bool exceededCpuLimits_;

public:
    const optional<int>& exitCode() const {
        return exitCode_;
    }

    const optional<string>& exitSignal() const {
        return exitSignal_;
    }

    bool exceededCpuLimits() const {
        return exceededCpuLimits_;
    }

    bool isSuccessful() const {
        return exitCode_ && exitCode_.value() == 0;
    }

    bool operator==(const ExecutionInfo& o) const {
        return tie(exitCode_, exitSignal_, exceededCpuLimits_)
               == tie(o.exitCode_, o.exitSignal_, o.exceededCpuLimits_);
    }
};

class ExecutionInfoBuilder {
private:
    ExecutionInfo subject_;

public:
    ExecutionInfoBuilder& setExitCode(int exitCode) {
        subject_.exitCode_ = optional<int>(exitCode);
        return *this;
    }

    ExecutionInfoBuilder& setExitSignal(string exitSignal) {
        subject_.exitSignal_ = optional<string>(exitSignal);
        return *this;
    }

    ExecutionInfoBuilder& setExceededCpuLimits(bool exceededCpuLimits) {
        subject_.exceededCpuLimits_ = exceededCpuLimits;
        return *this;
    }

    ExecutionInfo build() {
        return move(subject_);
    }
};

}
