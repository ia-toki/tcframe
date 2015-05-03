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

    virtual void setBaseDir(string baseDirName) = 0;
    virtual istream* openForReading(string name) = 0;
    virtual ostream* openForWriting(string name) = 0;
    virtual void remove(string name) = 0;
    virtual ExecutionResult execute(string command, string inputName, string outputName) = 0;
};

class UnixOperatingSystem : public OperatingSystem {
public:
    UnixOperatingSystem()
            : baseDirName(".") { }

    void setBaseDir(string baseDirName) {
        this->baseDirName = baseDirName;

        system(("rm -rf " + baseDirName).c_str());
        system(("mkdir -p " + baseDirName).c_str());
    }

    istream* openForReading(string name) {
        string filename = baseDirName + "/" + name;
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(string name) {
        string filename = baseDirName + "/" + name;
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    ExecutionResult execute(string command, string inputName, string outputName) {
        string inputFilename = baseDirName + "/" + inputName;
        string outputFilename = baseDirName + "/" + outputName;
        string errorFilename = baseDirName + "/_error.out";

        ExecutionResult result;
        int exitStatus = system((command + " < " + inputFilename + " > " + outputFilename + " 2> " + errorFilename).c_str());
        result.exitCode = WEXITSTATUS(exitStatus);
        result.outputStream = openForReading(outputName);
        result.errorStream = openForReadingAsStringStream(errorFilename);

        return result;
    }

    void remove(string name) {
        string filename = baseDirName + "/" + name;
        removeFile(filename);
    }

private:
    string baseDirName;

    void removeFile(string filename) {
        system(("rm -f " + filename).c_str());
    }

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
