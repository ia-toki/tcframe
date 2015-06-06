#ifndef TCFRAME_OS_H
#define TCFRAME_OS_H

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

using std::ifstream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

struct ExecutionResult {
    int exitStatus;
    istream* outputStream;
    istream* errorStream;
};

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
    virtual ExecutionResult execute(string command, string inputFilename, string outputFilename, string errorFilename) = 0;
};

class UnixOperatingSystem : public OperatingSystem {
public:
    UnixOperatingSystem()
            : timeLimitInSeconds(0),
              memoryLimitInMegabytes(0) { }

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

    ExecutionResult execute(string command, string inputFilename, string outputFilename, string errorFilename) {
        ostringstream sout;

        sout << "{ ";

        if (timeLimitInSeconds != 0 || memoryLimitInMegabytes != 0) {
            if (timeLimitInSeconds != 0) {
                sout << "ulimt -S -t " << timeLimitInSeconds << "; ";
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

private:
    int timeLimitInSeconds;
    int memoryLimitInMegabytes;

    istringstream* openForReadingAsStringStream(string filename) {
        ifstream file(filename);

        ostringstream buffer;
        buffer << file.rdbuf();

        removeFile(filename);

        return new istringstream(buffer.str());
    }
};

}

#endif
