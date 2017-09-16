#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>

#include "ExecutionRequest.hpp"
#include "ExecutionResult.hpp"
#include "tcframe/util.hpp"

using std::ifstream;
using std::ios;
using std::istream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

class OperatingSystem {
private:
    static constexpr const char* ERROR_FILENAME = "__tcframe_error.out";

public:
    virtual ~OperatingSystem() = default;

    virtual istream* openForReading(const string& filename) {
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    virtual ostream* openForWriting(const string& filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    virtual void closeOpenedStream(ios* stream) {
        delete stream;
    }

    virtual void forceMakeDir(const string& dirName) {
        runCommand("rm -rf " + dirName);
        runCommand("mkdir -p " + dirName);
    }

    virtual void removeFile(const string& filename) {
        runCommand("rm -rf " + filename);
    }

    virtual ExecutionResult execute(const ExecutionRequest& request) {
        ostringstream sout;

        sout << "{ ";

        if (request.timeLimit()) {
            sout << "ulimit -S -t " << request.timeLimit().value() << "; ";
        }

        // Note: this has no effect on OS X. No known workaround unfortunately.
        if (request.memoryLimit()) {
            sout << "ulimit -S -v " << request.memoryLimit().value() * 1024 << "; ";
        }

        sout << request.command() << "; }";

        if (request.inputFilename()) {
            sout << " < " << request.inputFilename().value();
        }
        if (request.outputFilename()) {
            sout << " 1> " << request.outputFilename().value();
        } else {
            sout << " 1> /dev/null";
        }
        sout << " 2> " << ERROR_FILENAME;

        ExecutionResultBuilder result;

        int exitValue = system(sout.str().c_str());
        int exitStatus = WEXITSTATUS(exitValue);

        if (exitStatus & (1<<7)) {
            result.setExitSignal(WTERMSIG(exitStatus));
        } else {
            result.setExitCode(exitStatus);
        }

        istream* errorStream = openForReading(ERROR_FILENAME);
        string errorString = StringUtils::streamToString(errorStream);
        closeOpenedStream(errorStream);
        result.setStandardError(errorString);

        return result.build();
    }

private:
    static void runCommand(const string& command) {
        system(command.c_str());
    }
};

}
