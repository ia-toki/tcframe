#pragma once

#include <istream>

using std::istream;

namespace tcframe { namespace experimental {

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

    const istream* outputStream() const {
        return outputStream_;
    }

    const istream* errorStream() const {
        return errorStream_;
    }
};

}}
