#pragma once

#include "tcframe/deprecated/os/ExecutionResult.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe_old {

class OperatingSystem {
public:
    virtual ~OperatingSystem() { }

    virtual istream* openForReading(string filename) = 0;
    virtual ostream* openForWriting(string filename) = 0;
    virtual void closeOpenedWritingStream(ostream* out) = 0;
    virtual void forceMakeDir(string dirName) = 0;
    virtual void removeFile(string filename) = 0;
    virtual void limitExecutionTime(int timeLimitInSeconds) = 0;
    virtual void limitExecutionMemory(int memoryLimitInMegabytes) = 0;
    virtual ExecutionResult execute(string id, string command, string inputFilename, string outputFilename, string errorFilename) = 0;
    virtual void combineMultipleTestCases(string testCaseBaseFilename, int testCasesCount) = 0;
};

}
