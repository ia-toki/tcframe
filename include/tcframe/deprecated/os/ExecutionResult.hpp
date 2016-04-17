#pragma once

#include <istream>

using std::istream;

namespace tcframe_old {

struct ExecutionResult {
    int exitStatus;
    istream* outputStream;
    istream* errorStream;
};

}
