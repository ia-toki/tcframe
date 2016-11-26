#pragma once

#include <istream>
#include <sstream>
#include <tuple>
#include <utility>

#include "ExecutionInfo.hpp"

using std::move;
using std::istream;
using std::istringstream;
using std::tie;

namespace tcframe {

struct ExecutionResult {
    friend class ExecutionResultBuilder;

private:
    ExecutionInfo info_;
    istream* outputStream_;
    istream* errorStream_;

public:
    ExecutionInfo info() const {
        return info_;
    }

    istream* outputStream() const {
        return outputStream_;
    }

    istream* errorStream() const {
        return errorStream_;
    }

    bool operator==(const ExecutionResult& o) const {
        return tie(info_) == tie(o.info_);
    }
};

class ExecutionResultBuilder {
private:
    ExecutionResult subject_;

public:
    ExecutionResultBuilder& setInfo(ExecutionInfo info) {
        subject_.info_ = info;
        return *this;
    }

    ExecutionResultBuilder& setOutputStream(istream* outputStream) {
        subject_.outputStream_ = outputStream;
        return *this;
    }

    ExecutionResultBuilder& setErrorStream(istream* errorStream) {
        subject_.errorStream_ = errorStream;
        return *this;
    }

    ExecutionResult build() {
        if (subject_.errorStream_ == nullptr) {
            subject_.errorStream_ = new istringstream();
        }
        if (subject_.outputStream_ == nullptr) {
            subject_.outputStream_ = new istringstream();
        }
        return move(subject_);
    }
};

}
