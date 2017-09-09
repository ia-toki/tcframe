#pragma once

#include <string>
#include <sys/signal.h>
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
    optional<int> exitSignal_;
    string standardError_;

public:
    ExecutionResult()
            : exitCode_(optional<int>(0)) {}

    const optional<int>& exitCode() const {
        return exitCode_;
    }

    const optional<int>& exitSignal() const {
        return exitSignal_;
    }

    const string& standardError() const {
        return standardError_;
    }

    bool isSuccessful() const {
        return exitCode_ && exitCode_.value() == 0;
    }

    bool operator==(const ExecutionResult& o) const {
        return tie(exitCode_, exitSignal_, standardError_)
               == tie(o.exitCode_, o.exitSignal_, standardError_);
    }
};

class ExecutionResultBuilder {
private:
    ExecutionResult subject_;

public:
    ExecutionResultBuilder& from(ExecutionResult other) {
        subject_ = other;
        return *this;
    }

    ExecutionResultBuilder& setExitCode(int exitCode) {
        subject_.exitCode_ = optional<int>(exitCode);
        subject_.exitSignal_ = optional<int>();
        return *this;
    }

    ExecutionResultBuilder& setExitSignal(int exitSignal) {
        subject_.exitCode_ = optional<int>();
        subject_.exitSignal_ = optional<int>(exitSignal);
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
