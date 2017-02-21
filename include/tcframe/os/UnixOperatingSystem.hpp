#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>

#include "ExecutionResult.hpp"
#include "OperatingSystem.hpp"
#include "tcframe/util.hpp"

using std::ifstream;
using std::ios;
using std::istream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

class UnixOperatingSystem : public OperatingSystem {
private:
    static constexpr const char* ERROR_FILENAME = "_error.out";

public:
    istream* openForReading(const string& filename) {
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(const string& filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    void closeOpenedStream(ios* stream) {
        delete stream;
    }

    void forceMakeDir(const string& dirName) {
        runCommand("rm -rf " + dirName);
        runCommand("mkdir -p " + dirName);
    }

    void removeFile(const string& filename) {
        runCommand("rm -rf " + filename);
    }

    ExecutionResult execute(const ExecutionRequest& request) {
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
            sout << " > " << request.outputFilename().value();
        } else {
            sout << " > /dev/null";
        }
        if (request.errorFilename()) {
            sout << " 2> " << request.errorFilename().value();
        } else {
            sout << " 2> " << ERROR_FILENAME;
        }

        ExecutionResultBuilder result;

        int exitValue = system(sout.str().c_str());
        int exitStatus = WEXITSTATUS(exitValue);

        if (WIFSIGNALED(exitStatus)) {
            int signal = WTERMSIG(exitStatus);
            result.setExitSignal(strsignal(signal));
            if (signal == SIGXCPU) {
                result.setExceededCpuLimits(true);
            }
        } else {
            result.setExitCode(exitStatus);
        }

        if (!request.errorFilename()) {
            istream* errorStream = openForReading(ERROR_FILENAME);
            string errorString = StringUtils::streamToString(errorStream);
            closeOpenedStream(errorStream);
            result.setStandardError(errorString);
        }

        return result.build();
    }

private:
    void runCommand(const string& command) {
        system(command.c_str());
    }
};

}
