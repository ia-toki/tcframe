#pragma once

#include "tcframe/deprecated/os/ExecutionResult.hpp"
#include "tcframe/deprecated/os/OperatingSystem.hpp"

#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

using std::ifstream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe_old {

class UnixOperatingSystem : public OperatingSystem {
public:
    istream* openForReading(string filename) {
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(string filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    void closeOpenedWritingStream(ostream* out) {
        delete out;
    }

    void forceMakeDir(string dirName) {
        system(("rm -rf " + dirName).c_str());
        system(("mkdir -p " + dirName).c_str());
    }

    void removeFile(string filename) {
        system(("rm -f " + filename).c_str());
    }

    void limitExecutionTime(int timeLimitInSeconds) {
        this->timeLimitInSeconds = timeLimitInSeconds;
    }

    void limitExecutionMemory(int memoryLimitInMegabytes) {
        this->memoryLimitInMegabytes = memoryLimitInMegabytes;
    }

    ExecutionResult execute(string, string command, string inputFilename, string outputFilename, string errorFilename) {
        ostringstream sout;

        sout << "{ ";

        if (timeLimitInSeconds != 0 || memoryLimitInMegabytes != 0) {
            if (timeLimitInSeconds != 0) {
                sout << "ulimit -S -t " << timeLimitInSeconds << "; ";
            }
            if (memoryLimitInMegabytes != 0) {
                sout << "ulimit -S -v " << memoryLimitInMegabytes * 1024 << "; ";
            }
        }

        sout << command << "; }";

        if (!inputFilename.empty()) {
            sout << " < " << inputFilename;
        }
        if (outputFilename.empty()) {
            sout << " > /dev/null";
        } else {
            sout << " > " << outputFilename;
        }
        if (errorFilename.empty()) {
            sout << " 2> /dev/null";
        } else {
            sout << " 2> " << errorFilename;
        }

        ExecutionResult result;
        int exitStatus = system(sout.str().c_str());
        result.exitStatus = WEXITSTATUS(exitStatus);

        if (outputFilename.empty()) {
            result.outputStream = new istringstream();
        } else {
            result.outputStream = openForReading(outputFilename);
        }

        if (errorFilename.empty()) {
            result.errorStream = new istringstream();
        } else {
            result.errorStream = openForReadingAsStringStream(errorFilename);
        }

        return result;
    }

    void combineMultipleTestCases(string testCaseBaseFilename, int testCasesCount) {
        ostringstream sout;
        sout << "echo " << testCasesCount << " > " << testCaseBaseFilename << ".in";
        sout << " && touch " << testCaseBaseFilename << ".out";
        system(sout.str().c_str());

        for (int i = 1; i <= testCasesCount; i++) {
            ostringstream sout2;
            sout2 << "tail -n +2 " << testCaseBaseFilename << "_" << i << ".in >> " << testCaseBaseFilename << ".in";
            sout2 << "&& cat " << testCaseBaseFilename << "_" << i << ".out >> " << testCaseBaseFilename << ".out";
            system(sout2.str().c_str());

            ostringstream sout3;
            sout3 << "rm " << testCaseBaseFilename << "_" << i << ".in ";
            sout3 << testCaseBaseFilename << "_" << i << ".out";
            system(sout3.str().c_str());
        }
    }

private:
    int timeLimitInSeconds = 0;
    int memoryLimitInMegabytes = 0;

    istringstream* openForReadingAsStringStream(string filename) {
        ifstream file(filename);

        ostringstream buffer;
        buffer << file.rdbuf();

        removeFile(filename);

        return new istringstream(buffer.str());
    }
};

}
