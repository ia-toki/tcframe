#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ExecutionResult {
    friend class ExecutionResultBuilder;

private:
    optional<int> exitCode_;
    optional<string> exitSignal_;
    bool exceededCpuLimits_;
    string standardError_;

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

    const string standardError() const {
        return standardError_;
    }

    bool isSuccessful() const {
        return exitCode_ && exitCode_.value() == 0;
    }

    bool operator==(const ExecutionResult& o) const {
        return tie(exitCode_, exitSignal_, exceededCpuLimits_, standardError_)
               == tie(o.exitCode_, o.exitSignal_, o.exceededCpuLimits_, standardError_);
    }
};

class ExecutionResultBuilder {
private:
    ExecutionResult subject_;

public:
    ExecutionResultBuilder() {
        subject_.exceededCpuLimits_ = false;
    }

    ExecutionResultBuilder& setExitCode(int exitCode) {
        subject_.exitCode_ = optional<int>(exitCode);
        return *this;
    }

    ExecutionResultBuilder& setExitSignal(string exitSignal) {
        subject_.exitSignal_ = optional<string>(exitSignal);
        return *this;
    }

    ExecutionResultBuilder& setExceededCpuLimits(bool exceededCpuLimits) {
        subject_.exceededCpuLimits_ = exceededCpuLimits;
        return *this;
    }

    ExecutionResultBuilder& setStandardError(string standardError) {
        subject_.standardError_ = standardError;
        return *this;
    }

    ExecutionResult build() {
        return move(subject_);
    }
};

}
