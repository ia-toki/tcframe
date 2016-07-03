#pragma once

#include <istream>
#include <tuple>

using std::istream;
using std::tie;

namespace tcframe {

struct ExecutionResult {
private:
    int exitStatus_;
    istream* outputStream_;
    istream* errorStream_;

public:
    ExecutionResult(int exitStatus, istream *outputStream, istream *errorStream)
            : exitStatus_(exitStatus)
            , outputStream_(outputStream)
            , errorStream_(errorStream)
    {}

    int exitStatus() const {
        return exitStatus_;
    }

    istream* outputStream() const {
        return outputStream_;
    }

    istream* errorStream() const {
        return errorStream_;
    }

    bool operator==(const ExecutionResult& o) const {
        return tie(exitStatus_) == tie(o.exitStatus_);
    }
};

}
