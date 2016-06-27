#pragma once

#include <istream>
#include <ostream>
#include <string>

#include "tcframe/os/ExecutionResult.hpp"

using std::istream;
using std::ostream;
using std::string;

namespace tcframe {

class OperatingSystem {
public:
    virtual ~OperatingSystem() {}

    virtual istream* openForReading(const string& filename) = 0;
    virtual ostream* openForWriting(const string& filename) = 0;
    virtual void closeOpenedWritingStream(ostream* out) = 0;
    virtual void forceMakeDir(const string& dirName) = 0;
    virtual void removeFile(const string& filename) = 0;
    virtual ExecutionResult execute(
            const string& command,
            const string& inputFilename,
            const string& outputFilename,
            const string& errorFilename) = 0;
    virtual void combineMultipleTestCases(const string& testCaseBaseFilename, int testCasesCount) = 0;
};

}
