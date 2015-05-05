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
    int exitCode;
    istream* outputStream;
    istream* errorStream;
};

class OperatingSystem {
public:
    virtual ~OperatingSystem() { }

    virtual istream* openForReading(string filename) = 0;
    virtual ostream* openForWriting(string filename) = 0;
    virtual void forceMakeDir(string dirName) = 0;
    virtual void removeFile(string filename) = 0;
    virtual ExecutionResult execute(string command, string inputFilename, string outputFilename) = 0;
};

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

    void forceMakeDir(string dirName) {
        system(("rm -rf " + dirName).c_str());
        system(("mkdir -p " + dirName).c_str());
    }

    void removeFile(string filename) {
        system(("rm -f " + filename).c_str());
    }

    ExecutionResult execute(string command, string inputFilename, string outputFilename) {
        ostringstream sout;

        sout << command;
        if (!inputFilename.empty()) {
            sout << " < " << inputFilename;
        }
        if (!outputFilename.empty()) {
            sout << " > " << outputFilename;
        }

        string errorFilename = "_error.out";
        sout << " 2> " << errorFilename;

        ExecutionResult result;
        int exitStatus = system(sout.str().c_str());
        result.exitCode = WEXITSTATUS(exitStatus);
        result.outputStream = openForReading(outputFilename);
        result.errorStream = openForReadingAsStringStream(errorFilename);

        return result;
    }

private:
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
