#pragma once

#include <istream>
#include <ostream>
#include <string>

#include "tcframe/experimental/os/ExecutionResult.hpp"

using std::istream;
using std::ostream;
using std::string;

namespace tcframe { namespace experimental {

class OperatingSystem {
public:
    virtual ~OperatingSystem() {}

    virtual istream* openForReading(string filename) = 0;
    virtual ostream* openForWriting(string filename) = 0;
    virtual void closeOpenedWritingStream(ostream* out) = 0;
    virtual void forceMakeDir(string dirName) = 0;
    virtual void removeFile(string filename) = 0;
    virtual ExecutionResult execute(string command, string inputFilename, string outputFilename, string errorFilename) = 0;
};

}}
