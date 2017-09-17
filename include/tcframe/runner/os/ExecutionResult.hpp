#pragma once

#include <cstring>
#include <map>
#include <string>
#include <tuple>
#include <utility>

#include "tcframe/exception.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ExecutionResult {
    friend class ExecutionResultBuilder;

private:
    optional<int> exitCode_ = optional<int>(0);
    optional<int> exitSignal_;
    string standardError_;

public:
    ExecutionResult() = default;

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

class ExecutionResults {
public:
    ExecutionResults() = delete;

    static FormattedError asFormattedError(const map<string, ExecutionResult>& executionResults) {
        vector<pair<int, string>> messages;
        for (const auto& entry : executionResults) {
            const string& key = entry.first;
            const ExecutionResult& executionResult = entry.second;

            if (!executionResult.isSuccessful()) {
                messages.emplace_back(0, "Execution of " + key + " failed:");
                if (executionResult.exitCode()) {
                    messages.emplace_back(1, "Exit code: " + StringUtils::toString(executionResult.exitCode().value()));
                    messages.emplace_back(1, "Standard error: " + executionResult.standardError());
                } else {
                    messages.emplace_back(1, "Exit signal: " + string(strsignal(executionResult.exitSignal().value())));
                }
            } else if (!executionResult.standardError().empty()) {
                messages.emplace_back(0, key + ": " + executionResult.standardError());
            }
        }
        return FormattedError(messages);
    }
};

class ExecutionResultBuilder {
private:
    ExecutionResult subject_;

public:
    ExecutionResultBuilder() = default;

    explicit ExecutionResultBuilder(ExecutionResult other) {
        subject_ = move(other);
    }

    ExecutionResultBuilder& setExitCode(int exitCode) {
        subject_.exitCode_ = optional<int>(exitCode);
        subject_.exitSignal_ = {};
        return *this;
    }

    ExecutionResultBuilder& setExitSignal(int exitSignal) {
        subject_.exitCode_ = {};
        subject_.exitSignal_ = optional<int>(exitSignal);
        return *this;
    }

    ExecutionResultBuilder& setStandardError(string standardError) {
        subject_.standardError_ = move(standardError);
        return *this;
    }

    ExecutionResult build() {
        return move(subject_);
    }
};

}
