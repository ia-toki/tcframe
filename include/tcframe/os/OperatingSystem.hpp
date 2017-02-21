#pragma once

#include <istream>
#include <ostream>
#include <string>

#include "ExecutionRequest.hpp"
#include "ExecutionResult.hpp"

using std::ios;
using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class OperatingSystem {
public:
    virtual ~OperatingSystem() {}

    virtual istream* openForReading(const string& filename) = 0;
    virtual ostream* openForWriting(const string& filename) = 0;
    virtual void closeOpenedStream(ios* stream) = 0;
    virtual void forceMakeDir(const string& dirName) = 0;
    virtual void removeFile(const string& filename) = 0;
    virtual ExecutionResult execute(const ExecutionRequest& request) = 0;
};

}
