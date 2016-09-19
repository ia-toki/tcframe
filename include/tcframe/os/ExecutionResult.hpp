#pragma once

#include <istream>
#include <tuple>

#include "ExecutionInfo.hpp"

using std::istream;
using std::tie;

namespace tcframe {

struct ExecutionResult {
private:
    ExecutionInfo info_;
    istream* outputStream_;
    istream* errorStream_;

public:
    ExecutionResult(ExecutionInfo info, istream* outputStream, istream* errorStream)
            : info_(info)
            , outputStream_(outputStream)
            , errorStream_(errorStream) {}

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

}
