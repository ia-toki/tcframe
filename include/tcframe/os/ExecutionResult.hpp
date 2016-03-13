#pragma once

#include <istream>

using std::istream;

namespace tcframe {

struct ExecutionResult {
    int exitStatus;
    istream* outputStream;
    istream* errorStream;
};

}